#pragma once
#include "ChessEngine/search/Searcher.h"
#include <gtest/gtest.h>

namespace search {
    class perftBase : public ::testing::Test {
        protected:
            bool longRuns = false;
            bool enableStartPosTest = false;
            bool enablePos2Test = true;
            bool enablePos3Test = false;
            bool enablePos5Test = false;

            std::string startPos;
            std::string posTwo;
            std::string posThree;
            std::string posFive;
            Searcher searcher;

            perftBase() : searcher(20) {}

            virtual void SetUp() override {
                startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
                posTwo = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R";
                posThree = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8";
                posFive = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R";
            }

            char colToChar(int col) {
                switch (col) {
                    case 0:
                        return 'h';
                    case 1:
                        return 'g';
                    case 2:
                        return 'f';
                    case 3:
                        return 'e';
                    case 4:
                        return 'd';
                    case 5:
                        return 'c';
                    case 6:
                        return 'b';
                    case 7:
                        return 'a';
                    default:
                        return 'x';
                }
            }

            std::string translateMoveToStr(game::Move move, bool whiteStarted) {
                if (move.isAnyCastle()) {
                    return move.isKingCastle() ? (whiteStarted ? "e1g1" : "e8g8") 
                                               : (whiteStarted ? "e1c1" : "e8c8");
                }
                
                int from = move.getBitIndexFrom();
                int to = move.getBitIndexTo();
                
                int fromRow = from / 8;
                int fromCol = from % 8;

                char fromColChar = colToChar(fromCol);

                int toRow = to / 8;
                int toCol = to % 8;
                char toColChar = colToChar(toCol);

                std::string moveStr = "";
                moveStr += fromColChar;
                moveStr += std::to_string(fromRow + 1);
                moveStr += toColChar;
                moveStr += std::to_string(toRow + 1);

                return moveStr;
            }

            std::vector<std::string> nodeCountPerFirstMoveAsStrVec(bool whiteStarted) {
                std::vector<std::string> nodeCountPerFirstMoveStrVec;
                int sum = 0;

                for (size_t i = 0; i < searcher._nodeCountPerFirstMove.size(); i++) {
                    if (searcher._firstMoves[i].getMove() != 0) {
                        std::string moveStr = translateMoveToStr(searcher._firstMoves[i], whiteStarted);
                        std::string nodeCountStr = std::to_string(searcher._nodeCountPerFirstMove[i]);
                        std::string moveNodeCountStr = moveStr + ": " + nodeCountStr;
                        nodeCountPerFirstMoveStrVec.push_back(moveNodeCountStr);

                        sum += searcher._nodeCountPerFirstMove[i];
                    }
                }

                nodeCountPerFirstMoveStrVec.push_back("Total: " + std::to_string(sum));

                return nodeCountPerFirstMoveStrVec;
            }
    };
}