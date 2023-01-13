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
Build SecureUxTheme and ThemeTool.

Options:
	clean, --clean		Delete bin/ and obj/ directories
	help,  --help 		Show this usage and eixt
	--            		Stop parsing arguments
EOF
	exit
fi

if [[ ${doClean} == 1 ]]; then
	echo Cleaning...
	rm -rf "./bin/" "./obj/"
	exit $?
fi

cd SecureUxTheme
	./build.sh
cd ..

cd ThemeTool
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

declare outdir objdir

# ThemeTool only supports x86 builds
for ARCH in ia32; do
	for DEBUG in 0 1; do
		outdir="../bin/${OUTDIR_ISDEBUG[${DEBUG}]}/${OUTDIR_ARCH[${ARCH}]}"
		objdir="../obj/${OUTDIR_ISDEBUG[${DEBUG}]}/${OUTDIR_ARCH[${ARCH}]}/SecureUxTheme"
		mkdir -p "${outdir}" "${objdir}"
		make TARGET="${TARGET_ARCH[${ARCH}]}" DEBUG="${DEBUG}" BIN="${outdir}" OBJ="${objdir}"
	done
done
