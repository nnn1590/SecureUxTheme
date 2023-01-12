#!/usr/bin/env bash
set -ue

cd "$(cd "$(dirname "${BASH_SOURCE[0]:-${0}}")" && pwd)"

declare -r BIN=SecureUxTheme.dll

declare -Ar TARGET_ARCH=(
	[ia32]=i686-w64-mingw32
	[amd64]=x86_64-w64-mingw32
)
declare -Ar OUTDIR_ARCH=(
	[ia32]=Win32
	[amd64]=x64
	[aarch64]=arm64
)
declare -Ar OUTDIR_ISDEBUG=(
	[0]=Release
	[1]=Debug
)

declare outdir

for ARCH in ia32 amd64; do
	for DEBUG in 0 1; do
		outdir="../bin/${OUTDIR_ISDEBUG[${DEBUG}]}/${OUTDIR_ARCH[${ARCH}]}"
		mkdir -p "${outdir}"
		make TARGET="${TARGET_ARCH[${ARCH}]}" DEBUG="${DEBUG}" OUTPUT="${outdir}/${BIN}"
	done
done
