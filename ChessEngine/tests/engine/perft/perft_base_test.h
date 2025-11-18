#pragma once

#include "engine/pickmove/move_picker.h"

#include "io/fen.h"

#include "model/position/board.h"

#include <cstdint>
#include <gtest/gtest.h>
#include <unordered_map>

namespace engine {

class perftBase : public ::testing::Test 
{

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

    MovePicker movePicker;

    perftBase() : movePicker(20) {}

    virtual void SetUp() override 
    {
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

    std::string translateMoveToStr(model::Move move, bool whiteStarted) 
    {
        if (move.is_any_castle()) {
            return move.is_king_castle() ? (whiteStarted ? "e1g1" : "e8g8") 
                                        : (whiteStarted ? "e1c1" : "e8c8");
        }
        
        int from = move.get_bit_index_from();
        int to = move.get_bit_index_to();
        
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

        if (move.is_any_promo()) {
            switch (move.get_flag()) {
                case model::Move::KNIGHT_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "n" : "N";
                    break;
                case model::Move::BISHOP_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "b" : "B";
                    break;
                case model::Move::ROOK_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "r" : "R";
                    break;
                case model::Move::QUEEN_PROMO_FLAG:
                    moveStr += (whiteStarted) ? "q" : "Q";
                    break;
                default:
                    break;
                case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "n" : "N";
                    break;
                case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "b" : "B";
                    break;
                case model::Move::ROOK_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "r" : "R";
                    break;
                case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
                    moveStr += (whiteStarted) ? "q" : "Q";
                    break;
            }
        }

        return moveStr;
    }

    fromToBitIndices translateStrToFromTo(std::string moveStr) 
    {
        fromToBitIndices fromTo;

        int fromRow = moveStr[1] - '1';
        int fromCol = 8 - (moveStr[0] - 'a') - 1;
        int toRow = moveStr[3] - '1';
        int toCol = 8 - (moveStr[2] - 'a') - 1;

        fromTo.from = fromRow * 8 + fromCol;
        fromTo.to = toRow * 8 + toCol;

        return fromTo;
    }

    model::Move moveFromStrAndFlag(std::string moveStr, int flag) 
    {
        fromToBitIndices fromTo = translateStrToFromTo(moveStr);
        return model::Move(fromTo.from, fromTo.to, flag);
    }

    std::unordered_map<std::string, uint64_t> nodeCountPerFirstMoveAsMap(bool whiteStarted) 
    {
        std::unordered_map<std::string, uint64_t> nodeCountPerFirstMoveMap{};
        int sum = 0;

        for (size_t i = 0; i < movePicker._nodeCountPerFirstMove.size(); i++) {
            if (movePicker._firstMoves[i].get_move() != 0) {
                std::string moveStr = translateMoveToStr(movePicker._firstMoves[i], whiteStarted);
                std::string nodeCountStr = std::to_string(movePicker._nodeCountPerFirstMove[i]);
                std::string moveNodeCountStr = moveStr + ": " + nodeCountStr;
                nodeCountPerFirstMoveMap[moveStr] = movePicker._nodeCountPerFirstMove[i];

                sum += movePicker._nodeCountPerFirstMove[i];
            }
        }

        nodeCountPerFirstMoveMap["searched"] = sum;

        return nodeCountPerFirstMoveMap;
    }

    void compareFirstMoveCountsToStockfish(
        const std::unordered_map<std::string, uint64_t> firstMoveCounts,
        const std::unordered_map<std::string, uint64_t> stockfishResults) 
    {
        std::ostringstream errors;
        bool hasErrors = false;

        for (const auto& moveCountPair : firstMoveCounts) {
            const std::string& move = moveCountPair.first;
            uint64_t count = moveCountPair.second;

            auto foundIt = stockfishResults.find(move);
            if (foundIt == stockfishResults.end()) {
                errors << "Move: " << move << " not found in stockfish results.\n";
                hasErrors = true;
            } else {
                uint64_t stockfishCount = foundIt->second;
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
};

} // namespace engine