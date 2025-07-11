#include <chesskit/piece_type.h>

#include <optional>
#include <print>

int main() {
  std::optional<chesskit::PieceType> empty;
  std::optional<chesskit::PieceType> rook = chesskit::PieceType::kRook;

  std::println("\"{}\" \"{}\"", empty, rook);
  std::println("\"{:?foo}\" \"{:?foo}\"", empty, rook);
  std::println("\"{:[u]}\" \"{:[u]}\"", empty, rook);
  std::println("\"{:[l]}\" \"{:[l]}\"", empty, rook);
  std::println("\"{:[l]?foo}\" \"{:[l]?foo}\"", empty, rook);
  std::println("\"{:bar[]}\" \"{:bar[]}\"", empty, rook);
  std::println("\"{:[]baz}\" \"{:[]baz}\"", empty, rook);
  std::println("\"{:bar[]baz}\" \"{:bar[]baz}\"", empty, rook);
  std::println("\"{:bar[]baz?foo}\" \"{:bar[]baz?foo}\"", empty, rook);
  std::println("\"{:bar[u]baz?foo}\" \"{:bar[u]baz?foo}\"", empty, rook);

  std::println("e7xd8{:=[u]}", empty);
  std::println("e7xd8{:=[u]}", rook);
}
