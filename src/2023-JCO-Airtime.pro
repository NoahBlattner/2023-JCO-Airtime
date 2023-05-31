#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT       += core gui

QT += svgwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2023-JCO-Airtime
TEMPLATE = app

#DEFINES += DEBUG_SPRITE_COUNT
#DEFINES += DEBUG_BRECT
#DEFINES += DEBUG_SHAPE
#DEFINES += DEPLOY # Pour une compilation dans un but de déploiement

SOURCES += main.cpp\
    mainfrm.cpp \
    gamescene.cpp \
    sprite.cpp \
    gamecore.cpp \
    resources.cpp \
    gameview.cpp \
    utilities.cpp \
    gamecanvas.cpp \
    spritetickhandler.cpp \
    AdvancedCollisionSprite.cpp \
    AnimatedSprite.cpp \
    Collectible.cpp \
    DashRefill.cpp \
    DirectionalEntityCollider.cpp \
    LevelLoader.cpp \
    LevelTrigger.cpp \
    PhysicsEntity.cpp \
    Player.cpp \
    Particle.cpp \
    MovingPlatform.cpp \

HEADERS  += mainfrm.h \
    gamescene.h \
    sprite.h \
    gamecore.h \
    resources.h \
    gameview.h \
    utilities.h \
    gamecanvas.h \
    spritetickhandler.h \
    AdvancedCollisionSprite.h \
    AnimatedSprite.h \
    Collectible.h \
    DashRefill.h \
    DirectionalEntityCollider.h \
    LevelLoader.h \
    LevelTrigger.h \
    PhysicsEntity.h \
    Player.h \
    Particle.h \
    MovingPlatform.h \


FORMS    += mainfrm.ui

