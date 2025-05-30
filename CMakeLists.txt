cmake_minimum_required(VERSION 3.19)
project(SberGame)

set(CMAKE_CXX_STANDARD 20)

#list(APPEND ${PROJECT_NAME}_HEADERS
#        includes/Game.h
#        includes/Team.h
#        includes/UnitFactory.h
#        includes/Attack.h
#        includes/Command.h
#
#        includes/logger/logger.h
#
#        includes/interfaces/IUnit.h
#        includes/interfaces/IAction.h
#        includes/interfaces/ISpecAction.h
#        includes/interfaces/ICanBeCloned.h
#        includes/interfaces/ICanBeHilled.h
#        includes/interfaces/ITeam.h
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
#
#        includes/team/TeamIterator.h
#        includes/team/UnitNumberManager.h
#        includes/team/TeamReport.h
#)

list(APPEND ${PROJECT_NAME}_SOURCES
        main.cpp
        src/Game.cpp
        src/UnitFactory.cpp
        src/Attack.cpp
        src/CompositeUnit.cpp

        src/logger/Logger.cpp

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

        src/team/Team.cpp
        src/team/TeamBuilder.cpp
        src/team/UnitNumberManager.cpp
        src/team/TeamReport.cpp
        )

add_executable(SberGame ${${PROJECT_NAME}_SOURCES})
target_include_directories(SberGame PUBLIC includes)