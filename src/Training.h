/*
    This file is part of Leela Zero.
    Copyright (C) 2017 Gian-Carlo Pascutto

    Leela Zero is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leela Zero is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Leela Zero.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRAINING_H_INCLUDED
#define TRAINING_H_INCLUDED

#include <string>
#include <utility>

#include "config.h"
#include "Network.h"
#include "UCTNode.h"

class TimeStep {
public:
    Network::NNPlanes planes;
    std::vector<float> probabilities;
    Color to_move;
    float net_winrate;
    float root_uct_winrate;
    float child_uct_winrate;
    int bestmove_visits;
};

class OutputChunker {
public:
    OutputChunker(const std::string& basename, bool compress = false, size_t chunk_size = CHUNK_SIZE);
    ~OutputChunker();
    void append(const std::string& str);

    // Group this many positions in a batch.
    static constexpr size_t CHUNK_SIZE = 150;
private:
    std::string gen_chunk_name() const;
    void flush_chunks();

    size_t m_step_count{0};
    size_t m_chunk_count{0};
    std::string m_buffer;
    std::string m_basename;
    bool m_compress{false};
    size_t chunk_size_;
};

class Training {
public:
    static void clear_training();
    static void dump_training(int game_score, const std::string& out_filename);
    static void dump_training(int game_score, OutputChunker& outchunker);
    static void dump_stats(const std::string& out_filename);
    static void record(const BoardHistory& state, Move move);
    static void record(const BoardHistory& state, UCTNode& node);

private:
    // Consider only every 1/th position in a game.
    // This ensures that positions in a chunk are from disjoint games.
    static constexpr size_t SKIP_SIZE = 16;

    static void dump_stats(OutputChunker& outchunker);
    static std::vector<TimeStep> m_data;
};

#endif
