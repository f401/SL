use super::{block::Vector2i, game::Game};


pub fn get_around(target: &Vector2i, game: &Game) -> Vec<Vector2i> {
    let mut result = Vec::<Vector2i>::new();

    if let Some(pos) = target.up(game) {
        result.push(pos);
    }

    if let Some(pos) = target.down() {
        result.push(pos);
    }

    if let Some(pos) = target.right(game) {
         result.push(pos);
    }

    if let Some(pos) = target.left() {
        result.push(pos);
    }
    
    return result;
}

pub fn get_around_with(target: &Vector2i, game: &Game) -> Vec<Vector2i> {
    let mut result = get_around(target, game);

    if let Some(pos) = target.up(game) {
        if let Some(pos_1) = pos.right(game) {
            result.push(pos_1);
        }
        if let Some(pos_2) = pos.left() {
            result.push(pos_2);
        }
    }

    if let Some(pos) = target.down() {
        if let Some(pos_1) = pos.right(game) {
            result.push(pos_1);
        }
        if let Some(pos_2) = pos.left() {
            result.push(pos_2);
        }
    }
    return result;
}
