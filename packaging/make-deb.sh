#!/bin/sh

#
# Generates DEB packages and uploads them to GemFury
#

cpack -G DEB || exit $?

if [ "$APPVEYOR_REPO_TAG" = "true" ]; then
  for pkg in ./*.deb; do
    filename="$(basename "$pkg")"
    curl -F package=@$filename https://$GEMFURY_TOKEN@push.fury.io/frabert/ || exit $?
  done
fi