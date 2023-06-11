mod block_game;

fn main() {
    let a = block_game::Game::new(block_game::Vector2i::new(10, 20));
    a.print();
}
