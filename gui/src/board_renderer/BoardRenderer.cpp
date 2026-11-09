#include "BoardRenderer.h"

using namespace godot;

void BoardRenderer::set_tilemaps(
        TileMapLayer* pieces_tilemap_layer,
        TileMapLayer* selections_tilemap_layer,
        TileMapLayer* red_selections_tilemap_layer,
        TileMapLayer* moves_tilemap_layer) {

    pieces_tilemap = pieces_tilemap_layer;
    selections_tilemap = selections_tilemap_layer;
    red_selections_tilemap = red_selections_tilemap_layer;
    moves_tilemap = moves_tilemap_layer;
}

void BoardRenderer::update_graphics() {
    unselect();
    move_gen->clear_moves();

    draw_board();
}

void BoardRenderer::clear_all() {
    pieces_tilemap->clear();
    selections_tilemap->clear();
    moves_tilemap->clear();
    clear_arrows_and_red_selections();
}

void BoardRenderer::clear_arrows_and_red_selections() {
    arrow_drawer->call_deferred("clear_arrows");
    red_selections_tilemap->clear();
}

void BoardRenderer::draw_board() {
    pieces_tilemap->clear();

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        Piece piece = state->get_piece_at(square);
        
        if (piece == PIECE_NONE) {
            continue;
        }

        Vector2 piece_atlas_coords = get_piece_atlas_coords(piece);
        
        set_tilemap_square(square, *pieces_tilemap, piece_atlas_coords);
    }
}

void BoardRenderer::draw_moves() {
    moves_tilemap->clear();

    draw_bitboard(move_gen->get_curr_moves(), *moves_tilemap, MOVE_ATLAS_COORDS);
}

void BoardRenderer::draw_bitboard(const uint64_t& bb, TileMapLayer& tilemap_to_use, const Vector2 atlas_coords_to_use) {
    tilemap_to_use.clear();

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        Square true_square = square;

        if (is_bit_1(bb, square)) {
            set_tilemap_square(square, tilemap_to_use, atlas_coords_to_use);
        }
    }
}

void BoardRenderer::draw_or_erase_arrow(const Square from, const Square to) {
    Vector2 from_position = get_square_coords(from) * SQUARE_PX_SIZE + Vector2(SQUARE_PX_SIZE, SQUARE_PX_SIZE) / 2;
    Vector2 to_position = get_square_coords(to) * SQUARE_PX_SIZE + Vector2(SQUARE_PX_SIZE, SQUARE_PX_SIZE) / 2;
    arrow_drawer->call_deferred("draw_or_erase_arrow", from_position, to_position);
}


void BoardRenderer::select(const Square square) {
    unselect();

    selected_square = square;
    
    set_tilemap_square(square, *selections_tilemap);

    draw_moves();
}

void BoardRenderer::unselect() {
    selected_square = SQ_NONE;
    selections_tilemap->clear();
    moves_tilemap->clear();
}

void BoardRenderer::hightlight_move(const Move& move) {
    set_tilemap_square(move.from, *selections_tilemap);
    set_tilemap_square(move.to, *selections_tilemap);
}

void BoardRenderer::toggle_red_selection(Square square) {
    Vector2 square_coords = get_square_coords(square);
    bool is_already_red_selected = red_selections_tilemap->get_cell_atlas_coords(square_coords) == RED_SELECTED_ATLAS_COORDS;

    if (is_already_red_selected) {
        red_selections_tilemap->erase_cell(square_coords);
    }
    else {
        red_selections_tilemap->set_cell(square_coords, 0, RED_SELECTED_ATLAS_COORDS);
    }
}

void BoardRenderer::set_tilemap_square(Square square, TileMapLayer& tilemap_to_use, Vector2 atlas_coords) {
    Vector2 square_coords = get_square_coords(square);

    if (board_flipped) {
        square_coords = get_square_coords(get_flipped_square(square));
    }

    tilemap_to_use.set_cell(square_coords, 0, atlas_coords);
}

void BoardRenderer::flip_board() {
    board_flipped = !board_flipped;

    draw_board();
    selections_tilemap->clear();
    moves_tilemap->clear();
}
    
Square BoardRenderer::get_flipped_square(const Square square) {
    return static_cast<Square>(SQ_63 - square);
}

Vector2 BoardRenderer::get_piece_atlas_coords(const Piece& piece) const {
    int piece_type_int = static_cast<int>(piece_to_piece_type[piece]);
    return is_white(piece) ? Vector2(piece_type_int, 0) : Vector2(piece_type_int, 1);
}

Vector2 BoardRenderer::get_square_coords(const Square square) const {
    int column = square % 8;
    int row = floor(square / 8);
    int flipped_row = 7 - row;

    return Vector2(column, flipped_row);
} 