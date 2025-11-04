#!/bin/bash
# Build solution for mov language problems

# Create a wrapper executable
cat > code << 'WRAPPER'
#!/bin/sh
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "${SCRIPT_DIR}/interpreter/interpreter" "${SCRIPT_DIR}/solutions/2076.mv" /dev/stdin
WRAPPER

chmod +x code
