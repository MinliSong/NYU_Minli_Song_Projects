#pragma once

#include <vector>
#include "sthread.h"
#include "Request.h"

static smutex_t printfMutex;


/* 
 * ------------------------------------------------------------------
 * Item -- 
 *
 *      This class represents a type of item in the inventory of the
 *      estore.  It keeps track of the number of units in stock, the
 *      price of each unit, etc.
 *
 *      The current price of an individual item is defined as the
 *      normal price of the item (i.e. the price field of Item) times
 *      1 - the current discount (i.e. the discount field of Item).
 *      When a customer tries to buy an item, the current price of
 *      the item should be used to determine the cost of the overall
 *      purchase.
 *
 *      If the particular item is not being offered by the store,
 *      then the valid field of the item in the inventory will be
 *      set to false.
 *
 * ------------------------------------------------------------------
 */
class Item {
    public:
    bool valid;
    int quantity;
    double price;
    double discount;
    smutex_t itemMutex;
    scond_t itemNonEmpty;

    Item();
    ~Item();

};


/* 
 * ------------------------------------------------------------------
 * EStore -- 
 *
 *      This class represents the current state of the estore.
 *      Customers and suppliers interact with the store through the
 *      methods of this class.
 *
 *      Items in the inventory are indexed by their item IDs.
 *
 *      The store discount should initially be set to 0.
 *      The shipping cost should initially be set to 3.
 *
 *      If fineMode is false, then this class functions strictly as
 *      a monitor. The buyItem method only functions in this mode.
 *
 *      If fineMode is true, simultaneous requests for:
 *          - addItem,
 *          - removeItem,
 *          - addStock,
 *          - discountItem
 *      that reference different item ids must process at the same
 *      time. The buyManyItems method only functions in this mode.
 *
 * ------------------------------------------------------------------
 */
class EStore {
    private:
    Item inventory[INVENTORY_SIZE];
    const bool fineMode;
    int shippingCost;
    double storeDiscount;
    smutex_t mutex;
    scond_t nonEmpty;
    scond_t enoughBudget;

    public:
      explicit EStore(bool enableFineMode);
    ~EStore();

    void buyItem(int item_id, double budget);
    void addItem(int item_id, int quantity, double price, double discount);
    void removeItem(int item_id);
    void addStock(int item_id, int count);
    void priceItem(int item_id, double price);
    void discountItem(int item_id, double discount);
    void setShippingCost(double price);
    void setStoreDiscount(double discount);

    void buyManyItems(std::vector<int>* item_ids, double budget);

    bool fineModeEnabled() const { return fineMode; }


};

