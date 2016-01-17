rmdir /S /Q Debug
rmdir /S /Q ipch
rmdir /S /Q Release
del /F /S GameEngine.sdf
del /F /S /A:H GameEngine.suo
rmdir /S /Q GameEngine\Debug
rmdir /S /Q GameEngine\Release
del /F /S file9.bin