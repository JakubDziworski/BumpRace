//
//  FbManager.h
//  BumpRace
//
//  Created by kuba on 30.08.2014.
//
//

#ifndef __BumpRace__FbManager__
#define __BumpRace__FbManager__
#include "cocos2d.h"
#include "screw.h"
class FbManager
{
private:
    FbManager();
    static FbManager *me;
public:
    static FbManager* getInstance();
    void loginExplicit();
    void logout();
    void loginImplicit();
    void addOnSessionChangedHandler(std::function<void(Session*)> fun);
};
#endif /* defined(__BumpRace__FbManager__) */
