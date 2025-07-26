#include <chesscxx/piece_type.h>

#include <optional>
#include <print>

auto main() -> int {
  std::optional<chesscxx::PieceType> empty;
  std::optional<chesscxx::PieceType> rook = chesscxx::PieceType::kRook;

  std::println(R"("{}" "{}")", empty, rook);
  std::println(R"("{:?foo}" "{:?foo}")", empty, rook);
  std::println(R"("{:[u]}" "{:[u]}")", empty, rook);
  std::println(R"("{:[l]}" "{:[l]}")", empty, rook);
  std::println(R"("{:[l]?foo}" "{:[l]?foo}")", empty, rook);
  std::println(R"("{:bar[]}" "{:bar[]}")", empty, rook);
  std::println(R"("{:[]baz}" "{:[]baz}")", empty, rook);
  std::println(R"("{:bar[]baz}" "{:bar[]baz}")", empty, rook);
  std::println(R"("{:bar[]baz?foo}" "{:bar[]baz?foo}")", empty, rook);
  std::println(R"("{:bar[u]baz?foo}" "{:bar[u]baz?foo}")", empty, rook);

  std::println("e7xd8{:=[u]}", empty);
  std::println("e7xd8{:=[u]}", rook);
}
