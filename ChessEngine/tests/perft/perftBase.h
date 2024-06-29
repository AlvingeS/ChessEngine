#pragma once
#include "ChessEngine/perft/Searcher.h"
#include "ChessEngine/utils/Fen.h"
#include "ChessEngine/game/BitBoards.h"
#include "ChessEngine/game/GameStateBitMasks.h"
#include "ChessEngine/game/SquaresLookup.h"

#include <gtest/gtest.h>
#include <unordered_map>
#include <sstream>
#include <regex>
#include <iostream>

namespace perft {
    class perftBase : public ::testing::Test {

        struct fromToBitIndices {
            int from;
            int to;
        };

        protected:
            bool longRuns = true;
            bool megaRuns = false;

            // Long run should be depth 6, mega run should be depth 7
            bool enableStartPosTest = true;
            int startPosMaxDepth = 5;

            // Long run should be depth 5
            bool enablePos2Test = true;
            int posTwoMaxDepth = 4;

            // Long run should be depth 7
            bool enablePos3Test = true;
            int posThreeMaxDepth = 6;

            // Long run should be depth 5
            bool enablePos5Test = true;
            int posFiveMaxDepth = 4;

            std::string startPos;
            std::string posTwo;
            std::string posThree;
            std::string posFive;

            Searcher searcher;

            perftBase() : searcher(20) {}

            virtual void SetUp() override {
                startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
                posTwo = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
                posThree = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
                posFive = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
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

                if (move.isAnyPromo()) {
                    switch (move.getFlag()) {
                        case game::Move::KNIGHT_PROMO_FLAG:
                            moveStr += (whiteStarted) ? "n" : "N";
                            break;
                        case game::Move::BISHOP_PROMO_FLAG:
                            moveStr += (whiteStarted) ? "b" : "B";
                            break;
                        case game::Move::ROOK_PROMO_FLAG:
                            moveStr += (whiteStarted) ? "r" : "R";
                            break;
                        case game::Move::QUEEN_PROMO_FLAG:
                            moveStr += (whiteStarted) ? "q" : "Q";
                            break;
                        default:
                            break;
                        case game::Move::KNIGHT_PROMO_CAPTURE_FLAG:
                            moveStr += (whiteStarted) ? "n" : "N";
                            break;
                        case game::Move::BISHOP_PROMO_CAPTURE_FLAG:
                            moveStr += (whiteStarted) ? "b" : "B";
                            break;
                        case game::Move::ROOK_PROMO_CAPTURE_FLAG:
                            moveStr += (whiteStarted) ? "r" : "R";
                            break;
                        case game::Move::QUEEN_PROMO_CAPTURE_FLAG:
                            moveStr += (whiteStarted) ? "q" : "Q";
                            break;
                    }
                }

                return moveStr;
            }

            fromToBitIndices translateStrToFromTo(std::string moveStr) {
                fromToBitIndices fromTo;

                int fromRow = moveStr[1] - '1';
                int fromCol = 8 - (moveStr[0] - 'a') - 1;
                int toRow = moveStr[3] - '1';
                int toCol = 8 - (moveStr[2] - 'a') - 1;

                fromTo.from = fromRow * 8 + fromCol;
                fromTo.to = toRow * 8 + toCol;

                return fromTo;
            }

            game::Move moveFromStrAndFlag(std::string moveStr, int flag) {
                fromToBitIndices fromTo = translateStrToFromTo(moveStr);
                return game::Move(fromTo.from, fromTo.to, flag);
            }

            std::unordered_map<std::string, long> nodeCountPerFirstMoveAsMap(bool whiteStarted) {
                std::unordered_map<std::string, long> nodeCountPerFirstMoveMap;
                int sum = 0;

                for (size_t i = 0; i < searcher._nodeCountPerFirstMove.size(); i++) {
                    if (searcher._firstMoves[i].getMove() != 0) {
                        std::string moveStr = translateMoveToStr(searcher._firstMoves[i], whiteStarted);
                        std::string nodeCountStr = std::to_string(searcher._nodeCountPerFirstMove[i]);
                        std::string moveNodeCountStr = moveStr + ": " + nodeCountStr;
                        nodeCountPerFirstMoveMap[moveStr] = searcher._nodeCountPerFirstMove[i];

                        sum += searcher._nodeCountPerFirstMove[i];
                    }
                }

                nodeCountPerFirstMoveMap["searched"] = sum;

                return nodeCountPerFirstMoveMap;
            }

            void compareFirstMoveCountsToStockfish(const std::unordered_map<std::string, long>& firstMoveCounts, const std::unordered_map<std::string, long>& stockfishResults) {
                std::ostringstream errors;
                bool hasErrors = false;

                for (const auto& moveCountPair : firstMoveCounts) {
                    const std::string& move = moveCountPair.first;
                    long count = moveCountPair.second;

                    auto foundIt = stockfishResults.find(move);
                    if (foundIt == stockfishResults.end()) {
                        errors << "Move: " << move << " not found in stockfish results.\n";
                        hasErrors = true;
                    } else {
                        long stockfishCount = foundIt->second;
                        if (count != stockfishCount) {
                            errors << "Move: " << move << " failed. Expected: " << stockfishCount << ", Got: " << count << ".\n";
                            hasErrors = true;
                        }
                    }
                }

                // If there were any errors, print them and fail the test
                if (hasErrors) {
                    std::cerr << errors.str();
                    ASSERT_TRUE(false);
                }
            }

            std::unordered_map<std::string, long> getStockFishPerftResults(std::string FEN, int depth) {
                std::unordered_map<std::string, long> results;
                std::ostringstream oss;

                oss << "echo \"position fen " << FEN << "\\ngo perft " << depth << "\\nquit\" | stockfish";
                std::string oss_str = oss.str();

                FILE* pipe = popen(oss_str.c_str(), "r");

                if (!pipe) {
                    std::cerr << "Couldn't open pipe to stockfish" << std::endl;
                    return results;
                }

                char buffer[256];
                std::string output = "";

                while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
                    output += buffer;
                }

                pclose(pipe);

                std::regex pattern(R"((\w+): (\d+))");
                std::smatch matches;
                while (std::regex_search(output, matches, pattern)) {
                    if (matches.size() == 3) { // Full match + 2 subgroups
                        std::string move = matches[1].str();
                        long count = std::stol(matches[2].str());
                        results[move] = count;
                    }
                    output = matches.suffix().str();
                }
            
                return results;
            }
    };
}