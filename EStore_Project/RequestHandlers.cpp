#include <iostream>
#include "sthread.h"
#include "Request.h"
#include "EStore.h"
using namespace std;
/*
 * ------------------------------------------------------------------
 * add_item_handler --
 *
 *      Handle an AddItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_item_handler(void *args)
{
    AddItemReq* req =  reinterpret_cast<AddItemReq*> (args);
   // cout<<"Handling AddItemReq: item_id - "<< req->item_id << " quantiy -  " << req->quantity << " price - " << req->price << " discount - " <<req->discount << endl;
    printf("Handling AddItemReq: item_id - %d quantiy -  %d  price - %f  discount - %f \n",req->item_id, req->quantity, req->price, req->discount);
    req->store->addItem(req->item_id, req->quantity, req->price, req->discount);

    return;
}

/*
 * ------------------------------------------------------------------
 * remove_item_handler --
 *
 *      Handle a RemoveItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
remove_item_handler(void *args)
{
    RemoveItemReq* req = reinterpret_cast<RemoveItemReq*> (args);
   // cout<<"Handling RemoveItemReq: item_id - "<<req->item_id <<endl;
    printf("Handling RemoveItemReq: item_id - %d \n",req->item_id);
    req->store->removeItem(req->item_id);
    return;
}

/*
 * ------------------------------------------------------------------
 * add_stock_handler --
 *
 *      Handle an AddStockReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_stock_handler(void *args)
{
    AddStockReq* req = reinterpret_cast<AddStockReq*> (args);
    //cout<<"Handling AddStockReq: item_id - "<<req->item_id << ", additional_stock -"<< req->additional_stock <<endl;
    printf("Handling AddStockReq: item_id - %d, additional_stock - %d \n", req->item_id, req->additional_stock);
    req->store->addStock(req->item_id,req->additional_stock);
    return;
}

/*
 * ------------------------------------------------------------------
 * change_item_price_handler --
 *
 *      Handle a ChangeItemPriceReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_price_handler(void *args)
{
    ChangeItemPriceReq* req = reinterpret_cast<ChangeItemPriceReq*> (args);
//  cout<<"Handling ChangeItemPriceReq: item_id - "<<req->item_id << ", new_price -"<< req->new_price <<endl;
    printf("Handling ChangeItemPriceReq: item_id - %d new_price - %f \n", req->item_id, req->new_price);
    req->store->priceItem(req->item_id,req->new_price);
    return;

}

/*
 * ------------------------------------------------------------------
 * change_item_discount_handler --
 *
 *      Handle a ChangeItemDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_discount_handler(void *args)
{
    ChangeItemDiscountReq* req = reinterpret_cast<ChangeItemDiscountReq*> (args);
   // cout<<"Handling ChangeItemDiscountReq: item_id - "<<req->item_id << ", new_discount -"<< req->new_discount <<endl;
    printf("Handling ChangeItemDiscountReq: item_id - %d , new_discount - %f \n", req->item_id, req->new_discount);
    req->store->discountItem(req->item_id,req->new_discount);
    return;

}

/*
 * ------------------------------------------------------------------
 * set_shipping_cost_handler --
 *
 *      Handle a SetShippingCostReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
set_shipping_cost_handler(void *args)
{
    SetShippingCostReq* req = reinterpret_cast<SetShippingCostReq*> (args);
//  cout<<"Handling SetShippingCostReq: new_cost - "<<req->new_cost << endl;
    printf("Handling SetShippingCostReq: new_cost - %f \n",req->new_cost);
    req->store->setShippingCost(req->new_cost);
    return;
}

/*
 * ------------------------------------------------------------------
 * set_store_discount_handler --
 *
 *      Handle a SetStoreDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
set_store_discount_handler(void *args)
{
    SetStoreDiscountReq* req = reinterpret_cast<SetStoreDiscountReq*> (args);
//  cout<<"Handling SetStoreDiscountReq: new_discount - "<<req->new_discount << endl;
    printf("Handling SetStoreDiscountReq: new_discount - %f \n", req->new_discount);
    req->store->setStoreDiscount(req->new_discount);
    return;
}

/*
 * ------------------------------------------------------------------
 * buy_item_handler --
 *
 *      Handle a BuyItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_item_handler(void *args)
{
    BuyItemReq* req = reinterpret_cast<BuyItemReq*> (args);
   // cout<<"Handling BuyItemReq: item_id - "<<req->item_id << ", budget - " <<req->budget << endl;
    printf("Handling BuyItemReq: item_id - %d, budget - %f \n", req->item_id, req->budget);
    req->store->buyItem(req->item_id, req->budget);
    return;
}

/*
 * ------------------------------------------------------------------
 * buy_many_items_handler --
 *
 *      Handle a BuyManyItemsReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_many_items_handler(void *args)
{
    BuyManyItemsReq* req = reinterpret_cast<BuyManyItemsReq*> (args);
    //cout<<"Handling BuyManyItemsReq:";
    printf("Handling BuyManyItemReq: item_ids - ");
    for (int i =0; i < (int) req->item_ids.size();i++){
	    printf(" %d", req->item_ids[i]);
    }
    cout<<", budget - " <<req->budget << endl;
    req->store->buyManyItems(&req->item_ids, req->budget);
    return;
}

/*
 * ------------------------------------------------------------------
 * stop_handler --
 *
 *      The thread should exit.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
stop_handler(void* args)
{
    printf("Handling StopHandlerReq: Quitting \n");
    sthread_exit();
}

