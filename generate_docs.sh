#!/bin/bash

# Generate Doxygen documentation for Points project

echo "Generating Doxygen documentation..."

# Check if doxygen is available
if ! command -v doxygen &> /dev/null; then
    echo "Error: Doxygen not found"
    echo "Please install Doxygen:"
    echo "  Ubuntu/Debian: sudo apt-get install doxygen"
    echo "  macOS: brew install doxygen"
    echo "  Or download from http://www.doxygen.nl/"
    exit 1
fi

# Create docs directory if it doesn't exist
mkdir -p docs

# Generate documentation
echo "Running doxygen..."
if doxygen Doxyfile; then
    echo "Documentation generated successfully!"
    echo "Open docs/html/index.html to view the documentation"
    
    # Ask if user wants to open the documentation
    read -p "Do you want to open the documentation now? (y/n): " OPEN
    if [[ $OPEN == "y" || $OPEN == "Y" ]]; then
        # Try different browsers/commands to open the file
        if command -v xdg-open &> /dev/null; then
            xdg-open docs/html/index.html
        elif command -v open &> /dev/null; then
            open docs/html/index.html
        else
            echo "Please open docs/html/index.html manually in your browser"
        fi
    fi
else
    echo "Error generating documentation"
    exit 1
fi
