#include "ChessEngine/search/Searcher.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>
#include <unordered_map>

namespace search {
    class perft : public ::testing::Test {
        protected:
            Searcher searcher;
            bool longRun = false;
            std::string pos2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R";
            std::string pos3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8";
            std::string pos5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R";

            bool enableStartPosTest = false;
            bool enablePos2Test = true;
            bool enablePos3Test = false;
            bool enablePos5Test = false;

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

            std::string translateMoveToStr(game::Move move) {
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

            std::vector<std::string> nodeCountPerFirstMoveAsStrVec() {
                std::vector<std::string> nodeCountPerFirstMoveStrVec;
                int sum = 0;

                for (size_t i = 0; i < searcher._nodeCountPerFirstMove.size(); i++) {
                    if (searcher._firstMoves[i].getMove() != 0) {
                        std::string moveStr = translateMoveToStr(searcher._firstMoves[i]);
                        std::string nodeCountStr = std::to_string(searcher._nodeCountPerFirstMove[i]);
                        std::string moveNodeCountStr = moveStr + ": " + nodeCountStr;
                        nodeCountPerFirstMoveStrVec.push_back(moveNodeCountStr);

                        sum += searcher._nodeCountPerFirstMove[i];
                    }
                }

                nodeCountPerFirstMoveStrVec.push_back("Total: " + std::to_string(sum));

                return nodeCountPerFirstMoveStrVec;
            }

        perft() : searcher(5) {}
    };

    TEST_F(perft, perft_starting_pos) {
        searcher.setMaxDepth(longRun ? 6 : 5);
        
        if (enableStartPosTest) {
            searcher.minimax(0, true, 0, true);
        }

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 20},
            {2, 400},
            {3, 8902},
            {4, 197281},
            {5, 4865609},
            {6, 119060324}
        };

        std::unordered_map<int, int> exepectedCaptures = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 34},
            {4, 1576},
            {5, 82719},
            {6, 2812008}
        };

        std::unordered_map<int, int> expectedEpCaptures = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 258},
            {6, 5248}
        };

        std::unordered_map<int, int> expectedCastling = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 0}
        };

        std::unordered_map<int, int> expectedPromotions = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 0}
        };

        std::unordered_map<int, int> expectedChecks = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 12},
            {4, 469},
            {5, 27351},
            {6, 809099}
        };

        std::unordered_map<int, int> expectedCheckmates = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 8},
            {5, 347},
            {6, 0} // Cant check for checkmate at max depth
        };

        if (enableStartPosTest) {
            for (int i = 1; i <= searcher.getMaxDepth(); i++) {
                ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
                ASSERT_EQ(searcher._captureCount[i], exepectedCaptures[i]);
                ASSERT_EQ(searcher._epCaptureCount[i], expectedEpCaptures[i]);
                ASSERT_EQ(searcher._castlingCount[i], expectedCastling[i]);
                ASSERT_EQ(searcher._promotionCount[i], expectedPromotions[i]);
                ASSERT_EQ(searcher._checkCount[i], expectedChecks[i]);
                
                if (i < searcher.getMaxDepth()) {
                    ASSERT_EQ(searcher._checkmateCount[i], expectedCheckmates[i]);
                }
            }
        }
    }

    TEST_F(perft, perft_pos2) {
        searcher.setMaxDepth(longRun ? 5 : 4);
        searcher.setBoardFromFen(pos2);

        if (enablePos2Test) {
            searcher.minimax(0, true, 0, true);
        }

        std::vector<std::string> strVec = nodeCountPerFirstMoveAsStrVec();

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 48},
            {2, 2039},
            {3, 97862},
            {4, 4085603},
            {5, 193690690}
        };

        std::unordered_map<int, int> expectedCaptures = {
            {0, 0},
            {1, 8},
            {2, 351},
            {3, 17102},
            {4, 757163},
            {5, 35043416}
        };

        std::unordered_map<int, int> expectedEpCaptures = {
            {0, 0},
            {1, 0},
            {2, 1},
            {3, 45},
            {4, 1929},
            {5, 73365}
        };

        std::unordered_map<int, int> expectedCastling = {
            {0, 0},
            {1, 2},
            {2, 91},
            {3, 3162},
            {4, 128013},
            {5, 4993637}
        };

        std::unordered_map<int, int> expectedPromotions = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 15172},
            {5, 8392}
        };

        std::unordered_map<int, int> expectedChecks = {
            {0, 0},
            {1, 0},
            {2, 3},
            {3, 993},
            {4, 25523},
            {5, 3309887}
        };

        std::unordered_map<int, int> expectedCheckmates = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 1},
            {4, 43},
            {5, 0} // Cant check for checkmate at max depth
        };

        if (enablePos2Test) {
            for (int i = 1; i <= searcher.getMaxDepth() - 1; i++) {
                ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
                ASSERT_EQ(searcher._captureCount[i], expectedCaptures[i]);
                ASSERT_EQ(searcher._epCaptureCount[i], expectedEpCaptures[i]);
                ASSERT_EQ(searcher._castlingCount[i], expectedCastling[i]);
                ASSERT_EQ(searcher._promotionCount[i], expectedPromotions[i]);
                ASSERT_EQ(searcher._checkCount[i], expectedChecks[i]);
                
                if (i < searcher.getMaxDepth()) {
                    ASSERT_EQ(searcher._checkmateCount[i], expectedCheckmates[i]);
                }       
            }
        }
    }

    TEST_F(perft, perft_pos3) {
        searcher.setMaxDepth(longRun ? 7 : 6);
        searcher.setBoardFromFen(pos3);

        if (enablePos3Test) {
            searcher.minimax(0, true, 0, true);
        }

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 14},
            {2, 191},
            {3, 2812},
            {4, 43238},
            {5, 674624},
            {6, 11030083},
            {7, 178633661}
        };

        std::unordered_map<int, int> expectedCaptures = {
            {0, 0},
            {1, 1},
            {2, 14},
            {3, 209},
            {4, 3348},
            {5, 52051},
            {6, 940350},
            {7, 14519036}
        };

        std::unordered_map<int, int> expectedEpCaptures = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 2},
            {4, 123},
            {5, 1165},
            {6, 33325},
            {7, 294874}
        };

        std::unordered_map<int, int> expectedCastling = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 0},
            {7, 0}
        };

        std::unordered_map<int, int> expectedPromotions = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {6, 7552},
            {7, 140024}
        };

        std::unordered_map<int, int> expectedChecks = {
            {0, 0},
            {1, 2},
            {2, 10},
            {3, 267},
            {4, 1680},
            {5, 52950},
            {6, 452473},
            {7, 12797406}
        };

        std::unordered_map<int, int> expectedCheckmates = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 0},
            {4, 17},
            {5, 0},
            {6, 2733},
            {7, 87}
        };

        if (enablePos3Test) {
            for (int i = 1; i <= searcher.getMaxDepth(); i++) {
                ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
                ASSERT_EQ(searcher._captureCount[i], expectedCaptures[i]);
                ASSERT_EQ(searcher._epCaptureCount[i], expectedEpCaptures[i]);
                ASSERT_EQ(searcher._castlingCount[i], expectedCastling[i]);
                ASSERT_EQ(searcher._promotionCount[i], expectedPromotions[i]);
                ASSERT_EQ(searcher._checkCount[i], expectedChecks[i]);
                
                if (i < searcher.getMaxDepth()) {
                    ASSERT_EQ(searcher._checkmateCount[i], expectedCheckmates[i]);
                }
            }
        }
    }

    TEST_F(perft, perft_pos5) {
        searcher.setMaxDepth(longRun ? 5 : 4);
        searcher.setBoardFromFen(pos5);
        searcher.getBoard().setHasCastled(false, true);

        if (enablePos5Test) {
            searcher.minimax(0, true, 0, true);
        }

        std::unordered_map<int, int> expectedNodes = {
            {0, 1},
            {1, 44},
            {2, 1486},
            {3, 62379},
            {4, 2103487},
            {5, 89941194}
        };

        if (enablePos5Test) {
            for (int i = 1; i <= searcher.getMaxDepth(); i++) {
                ASSERT_EQ(searcher._nodeCount[i], expectedNodes[i]);
            }
        }
    }
}

