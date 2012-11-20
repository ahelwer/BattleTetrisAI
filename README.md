# pason2012

## Introduction

Entry for 2012 Pason Bring on the Battle Coding Contest.

Contest website: http://codingcontest.pason.com/

The project was to create a simple AI which plays Battle Tetris against other AIs over a network. 
In competition this AI did quite well, losing in the single-elimination quarter-finals to the eventual overall winner.
A replay of an earlier victorious match is viewable here:

http://codingcontest.pason.com/tetris/index.html?mode=log&match_token=86c2006d-eb30-4a68-ac07-6221014f6f09

I am Raven, and my opponent is Notepad.
Team Notepad was overwhelmed with the number of rows being sent over, rendering their client nearly non-functional.

## Program Overview

The program communicates with a remote game server, receiving game state descriptions and sending move commands.
When a piece is put into play, Breadth-First Search is used to find all positions on the game board reachable by that piece.
Each reachable position is evaluated using a linear sum of 19 distinct criteria (with associated weights) to produce a position score.
The position with the highest such score is chosen, and move commands are sent to the server using an open-loop controller to move the piece into position.

The program employs advanced Machine Learning techniques to find the optimal weights for each of the 19 evaluation criteria.
The problem is one of maximizing the number of lines cleared for a random simulated Tetris game and set of weights with which to evaluate moves.
It can be formally stated as a numerical search for the global maxima of a 19-dimensional function over the range [-1, 1] along each dimension.
Harmony Search, a new stochastic metaheuristic Machine Learning algorithm, was used to rapidly converge on the mathematically optimal expected performance ratio of 1 line cleared for every 2.5 blocks given.
Once the Harmony Search converged (a process taking several hours on a cc2.8xlarge AWS EC2 instance), the resulting weights were hardcoded into the Battle Tetris client for use in move evaluation.

## Program Usage

Clone the repository and run scons to build the project (see <i>Required Libraries & Software</i> below).
Three targets will be built:

runTrainer - The Machine Learning training program. Takes several hours of runtime on a fast server to converge.

runTetris - The Tetris client itself. Expects a game server IP and match token as command line arguments.

runTests - Runs all unit, function, and integration tests using CppUnit.

Running the client itself requires the existence of a functioning gameserver.
More information on this as it is released by Pason.

If running on an AWS EC2 instance, run "python init.py" to use an HTTP GET request to get game details and automatically launch the Tetris client.

## Program Architecture

Overview of packages in this repository.

### main

Main entry points of each of the three targets built, which are described above.

### control

The main control loop for the Tetris client. 
Coordinates communication with the server, updating game state appropriately as messages are received.
Launches searches to determine best piece position as well as necessary sequence of moves to get there.

### server

Code which serializes and de-serializes messages to and from the server.
The ServerInterface class in particular provides a simple interface for networked communication.

### model

Classes used to represent, interact with, and modify internal game state.

### core

Tetris Oracle; functions for calculating the best move and the path to get there. Implementation of Harmony Search.

### trainer

Classes related to the training of the Tetris AI.

### util

Miscellaneous utility classes such as global constants and debug output functions.

### test

Unit, functional, and integration tests code.

Required Libraries & Software
-----------

The following are required to build and run the software:

ZeroMQ 2.2 - http://www.zeromq.org/intro:get-the-software

json-cpp 0.5.0 - http://sourceforge.net/projects/jsoncpp/

cppunit 1.12.1 - http://sourceforge.net/apps/mediawiki/cppunit/index.php?title=Main_Page#Download

Python 2.7.3 - http://www.python.org/download/

SCons 2.2.0 - http://www.scons.org/download.php

g++ 4.4.6 or newer - http://gcc.gnu.org/

Literature Used
-----------

<i>A New Heuristic Optimization Algorithm: Harmony Search</i>, Zong Woo Geem, Joong Hoon Kim and G.V. Loganathan. SIMULATION 2001 76: 60

<i>Music-Inspired Harmony Search Algorithm: Theory and Applications (Studies in Computational Intelligence)</i>, Zong Woo Geem. Springer, May 12, 2009

<i>On the Evolution of Artificial Tetris Players</i>, A. Boumaza. Proceedings of the 5th International Conference on Computational Intelligence and Games, 2009, pp. 381-393.

<i>Tetris Agent Optimization Using Harmony Search Algorithm</i>, Victor II M. Romero, Leonel L. Tomes and John Paul T. Yusiong. IJCSI International Journal of Computer Science Issues, Vol. 8, Issue 1, January 2011


