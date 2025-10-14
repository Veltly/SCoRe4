#!/bin/bash

python3 -m venv venv

source venv/bin/activate

pip install -r requirements.txt

chmod +x SurfaceGenerator.py

echo "Setup complete!"
echo "Activate the environment with: source venv/bin/activate"
echo "Then run: ./SurfaceGenerator.py"