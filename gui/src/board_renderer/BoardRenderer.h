#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node2d.hpp>

#include <godot_cpp/classes/tile_map_layer.hpp>

#include "../../chess/game_state/GameState.h"
#include "../../chess/constants/constants.h"
#include "../../chess/utils/utils.h"

#include "../configs/board_config.h"

namespace godot {

class BoardRenderer {
    
public:   
    BoardRenderer() = default;
    BoardRenderer(GameState* state, MoveGen* move_gen, Node2D* arrow_drawer) : 
        state(state), move_gen(move_gen), arrow_drawer(arrow_drawer) {}

    void set_tilemaps(TileMapLayer* pieces_tilemap, TileMapLayer* selections_tilemap, TileMapLayer* red_selections_tilemap, TileMapLayer* moves_tilemap);

    void update_graphics();
    void clear_all();
    void clear_arrows_and_red_selections();

    void draw_board();
    void draw_moves();
    void draw_bitboard(const uint64_t& bb, TileMapLayer& tilemap_to_use, const Vector2 atlas_coords_to_use);

    void draw_or_erase_arrow(const Square from, const Square to);

    void select(const Square square);
    void unselect();
    void toggle_red_selection(Square square);
    void set_tilemap_square(Square square, TileMapLayer& tilemap_to_use, Vector2 atlas_coords = SELECTED_ATLAS_COORDS);

    void hightlight_move(const Move& move);

    void flip_board();
    
    Square get_flipped_square(const Square square);
private:

    Vector2 get_piece_atlas_coords(const Piece& piece) const;
    Vector2 get_square_coords(const Square square) const;

    TileMapLayer* pieces_tilemap;
    TileMapLayer* selections_tilemap;
    TileMapLayer* red_selections_tilemap;
    TileMapLayer* moves_tilemap;

    GameState* state;
    MoveGen* move_gen;
    Node2D* arrow_drawer;

public:
    Square selected_square = SQ_NONE;

    bool board_flipped = false;
};

}