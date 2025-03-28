cmake_minimum_required(VERSION 3.19)
project(SberGame)

set(CMAKE_CXX_STANDARD 20)

#list(APPEND ${PROJECT_NAME}_HEADERS
#        includes/Game.h
#        includes/Team.h
#        includes/UnitFactory.h
#
#        includes/interfaces/IUnit.h
#        includes/interfaces/IAction.h
#        includes/interfaces/ISpecAction.h
#        includes/interfaces/ICanBeCloned.h
#        includes/interfaces/ICanBeHilled.h
#
#        includes/heros/HeavyHero.h
#        includes/heros/Hero.h
#        includes/heros/Archer.h
#        includes/heros/Hiller.h
#        includes/heros/Wizard.h
#        includes/heros/Wagenburg.h
#
#        includes/spec_actions/HillSpecAction.h
#        includes/spec_actions/CloneSpecAction.h
#
#        includes/attaks/NormalAttack.h
#        includes/attacks/DistanceAttack.h
#)

list(APPEND ${PROJECT_NAME}_SOURCES
        main.cpp
        src/Game.cpp
        src/UnitFactory.cpp
        src/team/Team.cpp
        src/team/TeamBuilder.cpp

        src/heros/HeavyHero.cpp
        src/heros/Hero.cpp
        src/heros/Archer.cpp
        src/heros/Hiller.cpp
        src/heros/Wizard.cpp
        src/heros/Wagenburg.cpp

        src/spec_actions/HillSpecAction.cpp
        src/spec_actions/CloneSpecAction.cpp

        src/attacks/NormalAttack.cpp
        src/attacks/DistanceAttack.cpp
        )

add_executable(SberGame ${${PROJECT_NAME}_SOURCES})
target_include_directories(SberGame PUBLIC includes)