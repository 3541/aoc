{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  packages = with pkgs; [ clang-tools rustc rust-analyzer ];
}
