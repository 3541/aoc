{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  packages = with pkgs; [
    (clang-tools.override { llvmPackages = llvmPackages_12; })
    cargo
    rustc
    rust-analyzer
    rustfmt
    lldb
  ];
}
