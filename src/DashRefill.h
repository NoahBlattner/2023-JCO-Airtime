//
// Created by blatnoa on 15.05.2023.
//

#ifndef INC_2023_JCO_AIRTIME_DASHREFILL_H
#define INC_2023_JCO_AIRTIME_DASHREFILL_H

#include "Collectible.h"

//! \brief A class that represents a dash refill collectible.
//!
//! This class is used to create a dash refill collectible.
//! When the collectible is collected, the player's dash is recharged.
class DashRefill : public Collectible {

public:
    explicit DashRefill(QGraphicsItem* pParent = nullptr);

protected:
    void onCollect(Player* player) override;
};


#endif //INC_2023_JCO_AIRTIME_DASHREFILL_H
