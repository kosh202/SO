{ pkgs }: {
	deps = [
   pkgs.strace
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}