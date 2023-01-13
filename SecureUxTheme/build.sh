#!/usr/bin/env bash
set -ue

cd "$(cd "$(dirname "${BASH_SOURCE[0]:-${0}}")" && pwd)"

declare arg
declare -i doClean=0 showHelp=0
for arg in "${@}"; do
	[[ "${arg}" == "--" ]] && break;
	[[ "${arg}" =~ ^(--)?help$ ]] && showHelp=1
	[[ "${arg}" =~ ^(--)?clean$ ]] && doClean=1
done

if [[ ${showHelp} == 1 ]]; then
	echo "Usage: ${0:-build.sh} [option]"
	cat << 'EOF'
Build SecureUxTheme(.dll).

Options:
	clean, --clean		Delete ../bin/(Debug|Release)/(Win32|x64)/SecureUxTheme.dll
	help,  --help 		Show this usage and eixt
	--            		Stop parsing arguments
EOF
	exit
fi

[[ ${doClean} == 1 ]] && echo Cleaning...

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
		if [[ ${doClean} == 1 ]]; then
			rm -f "${outdir}/${BIN}"
		else
			mkdir -p "${outdir}"
			make TARGET="${TARGET_ARCH[${ARCH}]}" DEBUG="${DEBUG}" OUTPUT="${outdir}/${BIN}"
		fi
	done
done
