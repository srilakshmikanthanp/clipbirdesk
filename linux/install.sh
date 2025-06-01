#!/bin/bash
# This Script is used to install or uninstall clipbird in the system
# pass --uninstall to uninstall the clipbird
# pass --help to get the help information
# pass --purge to uninstall the clipbird and remove the configuration files
# Only one option can be passed at a time

# check more than one argument is passed
if [ "$#" -gt 1 ]; then
  echo "Only one argument can be passed at a time"
  exit 1
fi

# Check if the script is running as root user
if [ "$EUID" -eq 0 ]; then
  echo "This script should not be run as root"
  exit 1
fi

# Commands
UNINSTALL="--uninstall"
HELP="--help"
PURGE="--purge"

# If help is passed then show the help information
if [ "$1" == "$HELP" ]; then
  echo "Usage: setup.sh [OPTION]"
  echo "Install or Uninstall the clipbird Application"
  echo ""
  echo "Options:"
  echo "  $UNINSTALL uninstall the clipbird"
  echo "  $PURGE     uninstall the clipbird completely"
  echo "  $HELP      Show this help information"
  exit 0
fi

# Define the List of Constants to pull the AppImage and Icon
ICON_URL="https://github.com/srilakshmikanthanp/clipbirdesk/blob/main/src/assets/images/logo.png?raw=true"
APP_URL="https://github.com/srilakshmikanthanp/clipbirdesk/releases/download/v1.3.3/clipbird-x86_64.AppImage"

# APP_HOME is where App is installed
APP_HOME="$HOME/.clipbird";
APP_NAME="clipbird.AppImage";

# Define the List of Variables
ICON_LOC="$APP_HOME/clipbird.png"
SH_LOC="$APP_HOME/clipbird.sh"
APP_LOC="$APP_HOME/$APP_NAME"
LOG_FILE="$APP_HOME/log.txt"

# Conf for .desktop file & auto start
APPDESKTOP_FILE="$HOME/.local/share/applications/clipbird.desktop"
AUTOSTART_DIR="$HOME/.config/autostart"
AUTOSTART_ENTRY="$AUTOSTART_DIR/clipbird.desktop"

# Check Applocation if running
if pgrep -f "$APP_LOC" > /dev/null; then
  echo "Stop the Application before Running this script"
  exit 1
fi

# Remove the AppImage file
if [ -f "$APP_LOC" ]; then
  echo "Removing AppImage..."
  rm "$APP_LOC"
fi

# Remove the Script file
if [ -f "$SH_LOC" ]; then
  echo "Removing clipbird..."
  rm "$SH_LOC"
fi

# Remove the Log file
if [ -f "$LOG_FILE" ]; then
  echo "Removing log file..."
  rm "$LOG_FILE"
fi

# Remove the Image
if [ -f "$ICON_LOC" ]; then
  echo "Removing icon..."
  rm "$ICON_LOC"
fi

# Remove the .desktop file if it exists
if [ -f "$APPDESKTOP_FILE" ]; then
  echo "Removing .desktop file..."
  rm "$APPDESKTOP_FILE"
fi

# remove the auto start file
if [ -L "$AUTOSTART_ENTRY" ]; then
  echo "Removing auto start file..."
  rm "$AUTOSTART_ENTRY"
fi

# Check if the user wants to uninstall the App
if [ "$1" == "$UNINSTALL" ]; then
  echo "Uninstall Complete"
  exit 0
fi

# Check if the user wants to purge the App
if [ "$1" == "$PURGE" ]; then
  rm -rf "$APP_HOME"
  echo "Purge Complete"
  exit 0
fi

# make the directory in HOME
mkdir -p $APP_HOME

# Exit immediately if any command fails
set -e

# Echo Information of AppImage
echo "Downloading $APP_URL to $APP_HOME"

# Download the AppImage
curl -L -o "$APP_LOC" "$APP_URL"

# Make the AppImage executable
chmod +x "$APP_LOC"

# Echo Information of Image
echo "Downloading $ICON_LOC to $APP_HOME"

# Download the Image
curl -L -o "$ICON_LOC" "$ICON_URL"

# Create the .desktop file
mkdir -p "$(dirname "$APPDESKTOP_FILE")"

# Fill the content for desktop file
echo "[Desktop Entry]
Name=clipbird
Comment=clipbird Application
Exec=$SH_LOC
Icon=$ICON_LOC
Terminal=false
Type=Application
Categories=Utility" > "$APPDESKTOP_FILE"

# Make the .desktop file executable
chmod +x "$APPDESKTOP_FILE"

# Fill the Shell script File
echo "$APP_LOC > "$LOG_FILE" 2>&1 &" > "$SH_LOC";

# Make the Shell script executable
chmod +x "$SH_LOC"

# make auto start dir
mkdir -p $AUTOSTART_DIR

# create the auto start entry
ln -s "$APPDESKTOP_FILE" "$AUTOSTART_ENTRY"

# start the Application
"$SH_LOC"

# Log the status of Installation
echo "Application Installed to $APP_HOME"
