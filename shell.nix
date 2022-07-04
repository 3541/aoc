{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  packages = with pkgs; [
    clang-tools
    cargo
    rustc
    rust-analyzer
    rustfmt
    lldb
    (python3.withPackages (p: with p; [ p.z3 ]))
  ];
}
