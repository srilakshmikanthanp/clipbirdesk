#!/bin/bash

# Export the platform theme env var
export QT_QPA_PLATFORMTHEME=xdgdesktopportal

# Get the directory of this script
HERE="$(dirname "$(readlink -f "${0}")")"

# Execute the Application
exec "${HERE}/usr/bin/clipbird" "$@"
