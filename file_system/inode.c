#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "bitmap.h"
#include "disk_map.h"
#include "passert.h"
#include "panic.h"
#include "inode.h"
#include "dir.h"


// Find the disk block number slot for the 'filebno'th block in inode 'ino'.
// Set '*ppdiskbno' to point to that slot.  The slot will be one of the
// ino->i_direct[] entries, an entry in the indirect block, or an entry
// in one of the indirect blocks referenced by the double-indirect block.
//
// When 'alloc' is set, this function will allocate an indirect block or
// a double-indirect block (and any indirect blocks in the double-indirect
// block) if necessary.
//
// Returns:
//	0 on success (but note that **ppdiskbno might equal 0).
//	-ENOENT if the function needed to allocate an indirect block, but
//		alloc was 0.
//	-ENOSPC if there's no space on the disk for an indirect block.
//	-EINVAL if filebno is out of range (it's >= N_DIRECT + N_INDIRECT +
//               N_DOUBLE).
//
// Hints:
//  - You may find it helpful to draw pictures.
//  - Don't forget to clear any block you allocate.
//  - Recall that diskaddr() converts from a disk block to an in-memory address
//  - You may end up writing code with a similar structure three times.
//  It may simplify your life to factor it into a helper function. 
int
inode_block_walk(struct inode *ino, uint32_t filebno, uint32_t **ppdiskbno, bool alloc)
{
	uint32_t *pdiskbno;
	//uint32_t r;
	//alloc direct block
	if(filebno < N_DIRECT){
	    *ppdiskbno = &ino->i_direct[filebno];
	    return 0;
	}
	//alloc indirect block
	else if(filebno < (N_INDIRECT + N_DIRECT)){
	    if(ino->i_indirect == 0){
		if(alloc == 0){
			return -ENOENT;
		}
	        ino->i_indirect = alloc_block();
	    	if(ino->i_indirect <= 0){
			return -ENOSPC;
		}
		//clear the block
		memset(diskaddr(ino->i_indirect),0,BLKSIZE);
	    }
	    pdiskbno = (uint32_t *)diskaddr(ino->i_indirect);
	    *ppdiskbno = &pdiskbno[filebno-N_DIRECT];
 	    return 0;
	}
	//alloc double block
	else if(filebno < (N_DIRECT + N_INDIRECT + N_DOUBLE)){

	    if(ino->i_double == 0){
		if(alloc == 0){
			return -ENOENT;
		}
	   	ino->i_double = alloc_block();
	    	if(ino->i_double <= 0){
			return -ENOSPC;
	    	}

		//clear the block
		memset(diskaddr(ino->i_double),0,BLKSIZE);
	    }
	    pdiskbno = (uint32_t *)diskaddr(ino->i_double);

	    int indirectno = ROUNDDOWN((filebno - N_DIRECT - N_INDIRECT),N_INDIRECT)/N_INDIRECT;
	    
	    if(pdiskbno[indirectno] == 0){
		if(alloc == 0){
			return -ENOENT;
		}
		pdiskbno[indirectno] = alloc_block();
	    	if(pdiskbno[indirectno] <= 0 || (super && pdiskbno[indirectno] >= super->s_nblocks)){
			return -ENOSPC;
	    	}
	
	        memset(diskaddr(pdiskbno[indirectno]),0,BLKSIZE);
	    }

            uint32_t *pindirect = (uint32_t *)diskaddr(pdiskbno[indirectno]);
	    *ppdiskbno = &pindirect[filebno - N_DIRECT - N_INDIRECT - indirectno*N_INDIRECT]; 
	    return 0;
	}
	else{
	    return -EINVAL;
	}
}


// Set *blk to the address in memory where the filebno'th block of
// inode 'ino' would be mapped.  Allocate the block if it doesn't yet
// exist.
//
// Returns 0 on success, < 0 on error.  Errors are:
//	-ENOSPC if a block needed to be allocated but the disk is full.
//	-EINVAL if filebno is out of rangeld/fsdriver.
//
// Hint: Use inode_block_walk and alloc_block.
int
inode_get_block(struct inode *ino, uint32_t filebno, char **blk)
{
	uint32_t *pdiskbno;
	int r = inode_block_walk(ino,filebno,&pdiskbno,true);

        if(filebno >= (N_DIRECT + N_INDIRECT + N_DOUBLE)){
	   return -EINVAL;
	}

	 if(r < 0){
	     return r;
	 }
        if(*pdiskbno == 0){

	     int blockno = alloc_block();
             if(blockno <= 0){
     	      	 return -ENOSPC;
	     }	
	     *pdiskbno = blockno;
     	}

	*blk = (char *) diskaddr(*pdiskbno);	 
 	return 0;


}
// Create "path".  On success set *pino to point at the inode and return 0.
// On error return < 0.
int
inode_create(const char *path, struct inode **pino)
{
	char name[NAME_MAX];
	int r;
	struct inode *dir;
	struct dirent *d;

	if ((r = walk_path(path, &dir, NULL, NULL, name)) == 0)
		return -EEXIST;
	if (r != -ENOENT || dir == 0)
		return r;
	if ((r = dir_alloc_dirent(dir, &d)) < 0)
		return r;
	if ((r = alloc_block()) < 0)
		return r;
	memset(diskaddr(r), 0, BLKSIZE);
	strcpy(d->d_name, name);
	d->d_inum = r;
	*pino = diskaddr(d->d_inum);
	inode_flush(dir);
	return 0;
}

// Open "path".  On success set *pino to point at the inode and return 0.
// On error return < 0.
int
inode_open(const char *path, struct inode **pino)
{
	return walk_path(path, 0, pino, 0, 0);
}

// Read count bytes from ino into buf, starting from seek position
// offset.  This meant to mimic the standard pread function.
// Returns the number of bytes read, < 0 on error.
ssize_t
inode_read(struct inode *ino, void *buf, size_t count, uint32_t offset)
{
	int r, bn;
	uint32_t pos;
	uint32_t *pblkno;
	char *blk;

	if (offset >= ino->i_size)
		return 0;

	count = MIN(count, ino->i_size - offset);

	for (pos = offset; pos < offset + count; ) {
		if ((r = inode_block_walk(ino, pos / BLKSIZE, &pblkno, 0)) < 0)
			switch (-r) {
			case ENOENT: // For sparse files.
				pblkno = NULL;
				break;
			default:
				return r;
			}
		bn = MIN(BLKSIZE - pos % BLKSIZE, offset + count - pos);
		// Handle sparse files.  If no block has been allocated for
		// this region of the file, fill the read buffer with zeroes.
		if (pblkno == NULL || *pblkno == 0)
			memset(buf, 0, bn);
		else {
			blk = diskaddr(*pblkno);
			memmove(buf, blk + pos % BLKSIZE, bn);
		}
		pos += bn;
		buf += bn;
	}

	return count;
}

// Write count bytes from buf into ino, starting at seek position
// offset.  This is meant to mimic the standard pwrite function.
// Extends the file if necessary.
// Returns the number of bytes written, < 0 on error.
int
inode_write(struct inode *ino, const void *buf, size_t count, uint32_t offset)
{
	int r, bn;
	uint32_t pos;
	char *blk;

	// Extend file if necessary
	if (offset + count > ino->i_size)
		if ((r = inode_set_size(ino, offset + count)) < 0)
			return r;

	for (pos = offset; pos < offset + count; ) {
		if ((r = inode_get_block(ino, pos / BLKSIZE, &blk)) < 0)
			return r;
		bn = MIN(BLKSIZE - pos % BLKSIZE, offset + count - pos);
		memmove(blk + pos % BLKSIZE, buf, bn);
		pos += bn;
		buf += bn;
	}

	return count;
}

// Remove a block from inode ino.  If it's not there, just silently succeed.
// Returns 0 on success, < 0 on error.
static int
inode_free_block(struct inode *ino, uint32_t filebno)
{
	int r;
	uint32_t *ptr;

	if ((r = inode_block_walk(ino, filebno, &ptr, 0)) < 0)
		switch (-r) {
		// Ignore not found error for sparse files.
		case ENOENT:
			return 0;
		default:
			return r;
		}
	if (*ptr) {
		free_block(*ptr);
		*ptr = 0;
	}
	return 0;
}

// Remove any blocks currently allocated for inode "ino" that would
// not be needed for an inode of size "newsize" (where newsize is smaller
// than ino->i_size).  Do not change ino->i_size.
//
// For both the old and new sizes, compute the number of blocks required,
// and then free the blocks from new_nblocks to old_nblocks.  If new_nblocks
// is no more than NDIRECT and the indirect block has been allocated, then
// free the indirect block.  Do the same for the double-indirect block if
// new_nblocks is no more than NDIRECT + NINDIRECT.  Don't forget to free
// the indirect blocks allocated in the double-indirect block!
//
// Hints:
// - Use inode_free_block to free all the data blocks, then use
// free_block to free the meta-data blocks (for example, the indirect block).
// - the ROUNDUP macro may be helpful
// - Note that we do not need to explicitly free the blocks pointed to
// by the indirect block (ask yourself: where are those blocks freed?)

static void
inode_truncate_blocks(struct inode *ino, uint32_t newsize)
{
	int r;
	uint32_t bno, old_nblocks, new_nblocks;
	new_nblocks = ROUNDUP(newsize,BLKSIZE)/BLKSIZE;
	old_nblocks = ROUNDUP(ino->i_size,BLKSIZE)/BLKSIZE;

 	for(bno = new_nblocks; bno < old_nblocks; bno++){//free all blocks
		r = inode_free_block(ino,bno);
		assert(r==0);
	}	

	// only have direct blocks
	if(old_nblocks <= N_DIRECT){
	    //do nothing
	}
	//have direct and indirect blcoks
	else if(old_nblocks <= (N_DIRECT + N_INDIRECT)){
            if(new_nblocks <= N_DIRECT && (ino->i_indirect != 0)){//free the meta-data for indirect block
		 free_block(ino->i_indirect);
		 ino->i_indirect = 0;
	    }
	}

	//have double blocks
	else{
	   if((new_nblocks <= N_DIRECT)){//free meta-data for double and indirect
		if(ino->i_indirect!=0){
		    free_block(ino->i_indirect);
		}
		if(ino->i_double!=0){
	        	uint32_t *pdiskbno = diskaddr(ino->i_double);
			for(r = 0; r < N_INDIRECT; ++r){
		    		if(pdiskbno[r]!=0){
		    			free_block(pdiskbno[r]);
 		    			pdiskbno[r] = 0;
		    		}

			}
			free_block(ino->i_double);
		}
 		ino->i_indirect = 0;
		ino->i_double = 0;
	   }
	   else if(new_nblocks <= (N_DIRECT + N_INDIRECT)){//free meta-data for all double 
		if(ino->i_double!=0){
	        	uint32_t *pdiskbno = diskaddr(ino->i_double);
			for(r = 0; r < N_INDIRECT; ++r){
		    		if(pdiskbno[r]!=0){
		    			free_block(pdiskbno[r]);
					pdiskbno[r] = 0;
		    		}
			}
			free_block(ino->i_double);
		}
		ino->i_double = 0;
	   }
	   else{//free meta-data for part of double 
		if(ino->i_double!=0){
	        	uint32_t *pdiskbno = diskaddr(ino->i_double);
			uint32_t new_nindirect = ROUNDUP((new_nblocks - N_DIRECT - N_INDIRECT),N_INDIRECT)/N_INDIRECT;
			uint32_t old_nindirect = ROUNDUP((old_nblocks - N_DIRECT - N_INDIRECT),N_INDIRECT)/N_INDIRECT;
			for(r = new_nindirect ; r < old_nindirect; ++r){
		   // flush_block(diskaddr(pdiskbno[r]));
		   		if(pdiskbno[r]!=0){
		   			free_block(pdiskbno[r]);
		    			pdiskbno[r] = 0;
				}
			}
		}
	   }
	}
	inode_flush(ino);

}

// Set the size of inode ino, truncating or extending as necessary.
int
inode_set_size(struct inode *ino, uint32_t newsize)
{
	if (ino->i_size > newsize)
		inode_truncate_blocks(ino, newsize);
	ino->i_size = newsize;
	flush_block(ino);
	return 0;
}

// Flush the contents and metadata of inode ino out to disk.  Loop over
// all the blocks in ino.  Translate the inode block number into a disk
// block number and then check whether that disk block is dirty.  If so,
// write it out.
void
inode_flush(struct inode *ino)
{
	int i;
	uint32_t *pdiskbno;

	for (i = 0; i < (ino->i_size + BLKSIZE - 1) / BLKSIZE; i++) {
		if (inode_block_walk(ino, i, &pdiskbno, 0) < 0 ||
		    pdiskbno == NULL || *pdiskbno == 0)
			continue;
		flush_block(diskaddr(*pdiskbno));
	}
	flush_block(ino);
	if (ino->i_indirect)
		flush_block(diskaddr(ino->i_indirect));
	if (ino->i_double) {
		// We have to flush every indirect block allocated in
		// addition to the double-indirect block itself.
		pdiskbno = diskaddr(ino->i_double);
		for (i = 0; i < N_INDIRECT; ++i)
			if (pdiskbno[i])
				flush_block(diskaddr(pdiskbno[i]));
		flush_block(diskaddr(ino->i_double));
	}
}

// Free disk resources reserved for an inode.  This should only be
// called in inode_unlink when an inode's link count hits 0.  Note
// that a block number (inum), and not a struct inode, is required as
// an argument to this function, as the block containing the inode
// must be freed as well.
static void
inode_free(uint32_t inum)
{
	struct inode *ino;

	ino = diskaddr(inum);
	assert(ino->i_nlink == 0);

	inode_truncate_blocks(ino, 0);
	flush_block(ino);
	free_block(inum);
}

// Unlink an inode by decrementing its link count and zeroing the name
// and inum fields in its associated struct dirent.  If the link count
// of the inode reaches 0, free the inode.
//
// Returns 0 on success, or -ENOENT if the file to be unlinked does
// not exist.
//
// Hint: Use walk_path and inode_free.  You will need to take advantage
// of walk_path setting the pdent parameter to point to the directory
// entry associated with the file to be unlinked.
int
inode_unlink(const char *path)
{
	// LAB: Your code here.
	//panic("inode_unlink not implemented");
	int r;
	struct inode *dir, *ino;
	struct dirent *dent;
	//char *lastelem;
	r = walk_path(path,&dir,&ino,&dent,NULL);
	if(r < 0){
	    return -ENOENT;
	}
 
	//inode side
	ino->i_nlink --;
	if(ino->i_nlink == 0){
	    inode_free(dent->d_inum);
	}

	//directory side
	memset(dent,0,sizeof(struct dirent));
	assert(dent->d_inum==0);
	assert(dent->d_name[0] == '\0');
	dent->d_name[0] = '\0';
	inode_flush(dir);
	inode_flush(ino);
	return 0;


}

// Link the inode at the location srcpath to the new location dstpath.
// Increment the link count on the inode.
//
// Returns 0 on success, < 0 on failure.  In particular, the function
// should fail with -EEXIST if a file exists already at dstpath.
//
// Hint: Use walk_path and dir_alloc_dirent.
int
inode_link(const char *srcpath, const char *dstpath)
{
	// LAB: Your code here.
       //panic("inode_link not implemented");

        int r;
	struct inode *srcdir, *srcino, *dstdir,*dstino;
	struct dirent *srcdent, *dstdent, *newdent;
	char srclastelem[NAME_MAX];
	char dstlastelem[NAME_MAX];
	//srcpath walk to find srcdent 
	r = walk_path(srcpath,&srcdir,&srcino,&srcdent,srclastelem);
	if(r < 0){
	    return r;
	}
	//dstpath walk to find dstdir
	r = walk_path(dstpath,&dstdir,&dstino,&dstdent,dstlastelem);
	if(r == 0){
	    return -EEXIST;
	}

	//directory side 
	//allocte a new dent
        r = dir_alloc_dirent(dstdir,&newdent);
	if(r < 0){
	    return r;
	}
	//set the dent 
	newdent->d_inum = srcdent->d_inum;         
	strcpy(newdent->d_name, dstlastelem);
        inode_flush(dstdir);

	//inode side
	srcino->i_nlink ++;
	inode_flush(srcino);

	return 0;


}

// Return information about the specified inode.
int
inode_stat(struct inode *ino, struct stat *stbuf)
{
	uint32_t i, nblocks, *pdiskbno;

	stbuf->st_mode = ino->i_mode;
	stbuf->st_size = ino->i_size;
	stbuf->st_blksize = BLKSIZE;
	for (i = 0, nblocks = 0; i < ROUNDUP(ino->i_size, BLKSIZE) / BLKSIZE; i++) {
		if (inode_block_walk(ino, i, &pdiskbno, 0) < 0)
			continue;
		if (*pdiskbno != 0)
			nblocks++;
	}
	stbuf->st_blocks = nblocks * (BLKSIZE / 512); // st_blocks unit is 512B.
	stbuf->st_nlink = ino->i_nlink;
	stbuf->st_mtime = ino->i_mtime;
	stbuf->st_atime = ino->i_atime;
	stbuf->st_ctime = ino->i_ctime;
	stbuf->st_uid = ino->i_owner;
	stbuf->st_gid = ino->i_group;
	stbuf->st_rdev = ino->i_rdev;

	return 0;
}
