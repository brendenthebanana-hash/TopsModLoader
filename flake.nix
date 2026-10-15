{
  description = "Tops: mod loader for Minecraft Bedrock Launcher (mcpelauncher)";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages.${system} = rec {
        tops-core = pkgs.stdenv.mkDerivation {
          pname = "tops-core";
          version = "0.1.0";
          src = ./.;
          nativeBuildInputs = [ pkgs.cmake ];
        };
        tops-cli = pkgs.writeShellScriptBin "tops" ''
          exec ${pkgs.python3}/bin/python3 ${./cli/tops} "$@"
        '';
        default = tops-core;
      };
      apps.${system}.default = {
        type = "app";
        program = "${self.packages.${system}.tops-cli}/bin/tops";
      };
      devShells.${system}.default = pkgs.mkShell {
        packages = [ pkgs.cmake pkgs.gcc pkgs.python3 ];
      };
    };
}
