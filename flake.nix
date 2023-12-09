{
  description = "AOC solutions.";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-23.11";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils, ... }:
    utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShell = pkgs.mkShell {
          packages = let o = pkgs.ocaml-ng.ocamlPackages_5_1;
          in with pkgs; [
            php
            nasm
            dotnet-sdk_8
            racket
            o.ocaml
            o.merlin
            o.utop
            o.ocamlformat
            clang
            lld
            gnustep.make
            clang-tools
            omnisharp-roslyn
          ];

          buildInputs = with pkgs; [ gnustep.base ];
        };
      });
}
