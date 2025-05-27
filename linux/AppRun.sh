#!/bin/bash

# Export the platform theme env var
export QT_QPA_PLATFORMTHEME=xdgdesktopportal

# Run the actual Qt binary inside the AppImage
HERE="$(dirname "$(readlink -f "${0}")")"

# Execute the Application
exec "${HERE}/usr/bin/clipbird" "$@"
