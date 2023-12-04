{
  description = "AOC solutions.";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-23.11";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils, ... }:
    utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system};
      in { devShell = pkgs.mkShell { packages = with pkgs; [ php nasm ]; }; });
}
