{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  packages = with pkgs; [
    (clang-tools.override { llvmPackages = llvmPackages_12; })
    cargo
    rustc
    rust-analyzer
    rustfmt
    lldb
    (python3.withPackages (p: with p; [ p.z3 ]))
  ];
}
