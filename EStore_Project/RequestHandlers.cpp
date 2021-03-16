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
    smutex_lock(&printfMutex);
    printf("Handling AddItemReq: item_id - %d quantiy -  %d  price - %f  discount - %f \n",req->item_id, req->quantity, req->price, req->discount);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling RemoveItemReq: item_id - %d \n",req->item_id);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling AddStockReq: item_id - %d, additional_stock - %d \n", req->item_id, req->additional_stock);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling ChangeItemPriceReq: item_id - %d new_price - %f \n", req->item_id, req->new_price);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling ChangeItemDiscountReq: item_id - %d , new_discount - %f \n", req->item_id, req->new_discount);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling SetShippingCostReq: new_cost - %f \n",req->new_cost);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling SetStoreDiscountReq: new_discount - %f \n", req->new_discount);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling BuyItemReq: item_id - %d, budget - %f \n", req->item_id, req->budget);
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);

    printf("Handling BuyManyItemReq: item_ids - ");
    for (int i =0; i < (int) req->item_ids.size();i++){
	    printf(" %d", req->item_ids[i]);
    }
   // printf("budget - \n", req->budget);
    printf(" \n");
    smutex_unlock(&printfMutex);
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
    smutex_lock(&printfMutex);
    printf("Handling StopHandlerReq: Quitting \n");
    smutex_unlock(&printfMutex);
    sthread_exit();
}

