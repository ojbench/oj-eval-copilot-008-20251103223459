#!/bin/bash
# Build solution for mov language problems

# Create a wrapper executable that runs the interpreter with the appropriate .mv file
cat > code << 'WRAPPER'
#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"
exec "${DIR}/interpreter/interpreter" "${DIR}/solutions/2076.mv" /dev/stdin
WRAPPER

chmod +x code
