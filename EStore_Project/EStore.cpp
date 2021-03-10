#include <cassert>

#include "EStore.h"
#include <iostream>
using namespace std;


Item::
Item() : valid(false)
{
    smutex_init(&itemMutex);
    scond_init(&itemNonEmpty);
}

Item::
~Item()
{ 
    smutex_destroy(&itemMutex);
    scond_destroy(&itemNonEmpty);
}


EStore::
EStore(bool enableFineMode)
    : fineMode(enableFineMode)
{   
    smutex_init(&mutex);
    scond_init(&nonEmpty);
    scond_init(&enoughBudget);

    shippingCost =3;
    storeDiscount = 0;
}

EStore::
~EStore()
{
   smutex_destroy(&mutex);
   scond_destroy(&nonEmpty);
   scond_destroy(&enoughBudget);
}

/*
 * ------------------------------------------------------------------
 * buyItem --
 *
 *      Attempt to buy the item from the store.
 *
 *      An item can be bought if:
 *          - The store carries it.
 *          - The item is in stock.
 *          - The cost of the item plus the cost of shipping is no
 *            more than the budget.
 *
 *      If the store *does not* carry this item, simply return and
 *      do nothing. Do not attempt to buy the item.
 *
 *      If the store *does* carry the item, but it is not in stock
 *      or its cost is over budget, block until both conditions are
 *      met (at which point the item should be bought) or the store
 *      removes the item from sale (at which point this method
 *      returns).
 *
 *      The overall cost of a purchase for a single item is defined
 *      as the current cost of the item times 1 - the store
 *      discount, plus the flat overall store shipping fee.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
buyItem(int item_id, double budget)
{   
    smutex_lock(&mutex);
    assert(!fineModeEnabled());
    if(item_id > (int)sizeof(inventory)-1){
	cout<<"item overflow"<<endl;
	smutex_unlock(&mutex);
        return;
    }

    Item *item = &inventory[item_id];
    if(item->valid==false){
     //  cout<<"Item is unvalid now!"<<endl;
       smutex_unlock(&mutex);
       return;
    }
    else{
       double cost = item->price*(1-item->discount)*(1-storeDiscount);
       while((item->quantity == 0) ||(budget < cost)){
          scond_wait(&nonEmpty,&mutex);
	 // scond_wait(&enoughBudget,&mutex);

       }

       item->quantity --;
       budget = budget - cost;

    }
    //cout<<"buy an item success"<<endl;
    smutex_unlock(&mutex);
    return;

}

/*
 * ------------------------------------------------------------------
 * buyManyItem --
 *
 *      Attempt to buy all of the specified items at once. If the
 *      order cannot be bought, give up and return without buying
 *      anything. Otherwise buy the entire order at once.
 *
 *      The entire order can be bought if:
 *          - The store carries all items.
 *          - All items are in stock.
 *          - The cost of the the entire order (cost of items plus
 *            shipping for each item) is no more than the budget.
 *
 *      If multiple customers are attempting to buy at the same
 *      time and their orders are mutually exclusive (i.e., the
 *      two customers are not trying to buy any of the same items),
 *      then their orders must be processed at the same time.
 *
 *      For the purposes of this lab, it is OK for the store
 *      discount and shipping cost to change while an order is being
 *      processed.
 *
 *      The cost of a purchase of many items is the sum of the
 *      costs of purchasing each item individually. The purchase
 *      cost of an individual item is covered above in the
 *      description of buyItem.
 *
 *      Challenge: For bonus points, implement a version of this
 *      method that will wait until the order can be fulfilled
 *      instead of giving up. The implementation should be efficient
 *      in that it should not wake up threads unecessarily. For
 *      instance, if an item decreases in price, only threads that
 *      are waiting to buy an order that includes that item should be
 *      signaled (though all such threads should be signaled).
 *
 *      Challenge: For bonus points, ensure that the shipping cost
 *      and store discount does not change while processing an
 *      order.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
buyManyItems(vector<int>* item_ids, double budget)
{
//    assert(fineModeEnabled());
//    smutex_lock(&mutex);
//    int i; 
//    Item items[item_ids->size()];
//    double itemsCost;
//    const double storeDis = storeDiscount;
//    const double shippingC = shippingCost;
//    for(i = 0; i < (int)item_ids->size(); i++){
//	if((*item_ids)[i] > (int)sizeof(inventory - 1)){
//	    cout<<"Inventory overflow"<<endl;
//	}
//
//	items[i] = inventory[(*item_ids)[i]];
//	if(items[i].valid == false){
//	    cout<<"Item is unvalid now"<<endl;
//	    smutex_unlock(&mutex);
//	    return;
//	}
//	itemsCost = itemsCost + items[i].price*(1-items[i].discount);
//    }
//
//    for(i =0; i < (int)sizeof(items); i++){
//	while( (items[i].quantity == 0) || ((itemsCost*(1-storeDis) + shippingC)) > budget){ 
// 	    scond_wait(&nonEmpty, &mutex);
//	    scond_wait(&enoughBudget, &mutex);
//	    itemsCost = 0;
//	    for(int n =0; n <(int)sizeof(items); n++){
//		itemsCost = itemsCost + items[i].price*(1-items[i].discount);
//	    }
//	}
//        
//
//    }
//
//    for(i = 0; i <(int)sizeof(items); i++){
//	items[i].quantity --;
//    }
//
//    budget = budget - itemsCost*(1 - storeDis) + shippingC;
//
//
//
//
//
//    smutex_unlock(&mutex);
    return;
    
}

/*
 * ------------------------------------------------------------------
 * addItem --
 *
 *      Add the item to the store with the specified quantity,
 *      price, and discount. If the store already carries an item
 *      with the specified id, do nothing.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addItem(int item_id, int quantity, double price, double discount)
{

    smutex_lock(&mutex); //normal mode
 
    Item *item = &inventory[item_id];

   // smutex_lock(&item->itemMutex);

    if(item->valid == true){
       	smutex_unlock(&mutex); // normal mode
        return;
    }
    
    smutex_lock(&item->itemMutex);//fine mode

    item->quantity = quantity;
    item->price = price;
    item->discount = discount;
    item->valid = true;

    smutex_unlock(&item->itemMutex);//fine mode

    smutex_unlock(&mutex); 

    return;
}

/*
 * ------------------------------------------------------------------
 * removeItem --
 *
 *      Remove the item from the store. The store no longer carries
 *      this item. If the store is not carrying this item, do
 *      nothing.
 *
 *      Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
removeItem(int item_id)
{
 
    smutex_lock(&mutex);

    Item *item = &inventory[item_id];
  
    if(item->valid == false){
	smutex_unlock(&mutex);
	return;
    }
 
    smutex_lock(&item->itemMutex);//fine mode

    item->valid = false;

    smutex_unlock(&item->itemMutex);//fine mode


    scond_broadcast(&nonEmpty,&mutex);
    scond_broadcast(&enoughBudget, &mutex);

    smutex_unlock(&mutex);
 
    return;
}

/*
 * ------------------------------------------------------------------
 * addStock --
 *
 *      Increase the stock of the specified item by count. If the
 *      store does not carry the item, do nothing. Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addStock(int item_id, int count)
{    
   // cout<<"Add stock"<<endl;
    smutex_lock(&mutex);
    Item *item = &inventory[item_id];

    if(item->valid == false){
	smutex_unlock(&mutex);
	return;
    }

    smutex_lock(&item->itemMutex); //fine mode

    item->quantity = item->quantity + count;

    smutex_unlock(&item->itemMutex);//fine mode

    scond_broadcast(&nonEmpty,&mutex);
    scond_broadcast(&enoughBudget, &mutex);

    smutex_unlock(&mutex);
    return;
}

/*
 * ------------------------------------------------------------------
 * priceItem --
 *
 *      Change the price on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item price decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
priceItem(int item_id, double price)
{ 
   // cout<<"set item price"<<endl;
    smutex_lock(&mutex);
    Item *item = &inventory[item_id];

    if(item->valid == false){
	smutex_unlock(&mutex);
	return;
    }

    smutex_lock(&item->itemMutex);//fine mode

    item->price = price;
    scond_broadcast(&nonEmpty,&mutex);
    scond_broadcast(&enoughBudget, &mutex);

    smutex_unlock(&item->itemMutex);//fine mode

    smutex_unlock(&mutex);
    return;
}

/*
 * ------------------------------------------------------------------
 * discountItem --
 *
 *      Change the discount on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item discount increased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
discountItem(int item_id, double discount)
{
   // cout<<"set item discount"<<endl;
    smutex_lock(&mutex);
    Item *item = &inventory[item_id];

    if(item->valid == false){
 	smutex_unlock(&mutex);
	return;
    }
    
    if(item->discount > discount){
        item->discount = discount;
	smutex_unlock(&mutex);
	return;
    }

    smutex_lock(&item->itemMutex);//fine mode

    item->discount = discount;
    scond_broadcast(&nonEmpty,&mutex);
    scond_broadcast(&enoughBudget, &mutex);

    smutex_unlock(&item->itemMutex); //fine mode

    smutex_unlock(&mutex);
    return;
}

/*
 * ------------------------------------------------------------------
 * setShippingCost --
 *
 *      Set the per-item shipping cost. If the shipping cost
 *      decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setShippingCost(double cost)
{   
   // cout<<"set shipping cost"<<endl;
    smutex_lock(&mutex);
    
    if(shippingCost < cost){
	shippingCost = cost;
 	smutex_unlock(&mutex);
	return;
    }   

    shippingCost = cost;
    scond_broadcast(&nonEmpty,&mutex);
    scond_broadcast(&enoughBudget, &mutex);

    smutex_unlock(&mutex);
    return;
}

/*
 * ------------------------------------------------------------------
 * setStoreDiscount --
 *
 *      Set the store discount. If the discount increased, wake any
 *      waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setStoreDiscount(double discount)
{    
    //cout<<"set discount"<<endl;

    smutex_lock(&mutex);
    
    if(storeDiscount > discount){
	storeDiscount = discount;
	smutex_unlock(&mutex);
	return;
    }   

    storeDiscount = discount;
    scond_broadcast(&nonEmpty,&mutex);
    scond_broadcast(&enoughBudget, &mutex);

    smutex_unlock(&mutex);
    return;
}


