#!/bin/bash

EXE=./scop

echo "============================================================"
echo " -- TEST 1: Only help --"
echo "===="
echo "1.|   $EXE --help"
echo "===="
$EXE --help
echo ""

echo "============================================================"
echo " -- TEST 2: Object file + width and height (short flags) --"
echo "===="
echo "1.|   $EXE -f model.obj -w 800 -h 600"
echo "===="
$EXE -f model.obj -w 800 -h 600
echo ""

echo "============================================================"
echo " -- TEST 3: Object file + width and height (long flags) --"
echo "===="
echo "1.|   $EXE --file model.obj --width 1024 --height 768"
echo "===="
$EXE --file model.obj --width 1024 --height 768
echo ""

echo "============================================================"
echo " -- TEST 4: Vertex shader and texture shader (short flags) --"
echo "===="
echo "1.|   $EXE -vs vs.glsl -ts ts.glsl"
echo "===="
$EXE -vs vs.glsl -ts ts.glsl
echo ""

echo "============================================================"
echo " -- TEST 5: Vertex shader and texture shader (long flags) --"
echo "===="
echo "1.|   $EXE --vertexShader vs.glsl --textureShader ts.glsl"
echo "===="
$EXE --vertexShader vs.glsl --textureShader ts.glsl
echo ""

echo "============================================================"
echo " -- TEST 6: Texture file --"
echo "===="
echo "1.|   $EXE -t diffuse.png"
echo "===="
$EXE -t diffuse.png
echo "===="
echo "2.|   $EXE --texture diffuse.png"
echo "===="
$EXE --texture diffuse.png
echo ""

echo "============================================================"
echo " -- TEST 7: Full combination (short flags) --"
echo "===="
echo "1.|   $EXE -f model.obj -w 1024 -h 768 -vs vs.glsl -ts ts.glsl -t diffuse.png"
echo "===="
$EXE -f model.obj -w 1024 -h 768 -vs vs.glsl -ts ts.glsl -t diffuse.png
echo ""

echo "============================================================"
echo " -- TEST 8: Full combination (long flags) --"
echo "===="
echo "1.|   $EXE --file model.obj --width 640 --height 480 --vertexShader vs.glsl --textureShader ts.glsl --texture diffuse.png"
echo "===="
$EXE --file model.obj --width 640 --height 480 --vertexShader vs.glsl --textureShader ts.glsl --texture diffuse.png
echo ""

echo "============================================================"
echo " -- TEST 9: Mix of short + long flags --"
echo "===="
echo "1.|   $EXE -f model.obj --width 1280 --height 720 -vs vs.glsl --textureShader ts.glsl -t diffuse.png"
echo "===="
$EXE -f model.obj --width 1280 --height 720 -vs vs.glsl --textureShader ts.glsl -t diffuse.png
echo ""

echo "============================================================"
echo " -- TEST 10: Only dimensions and texture --"
echo "===="
echo "1.|   $EXE -w 1920 -h 1080 -t texture.png"
echo "===="
$EXE -w 1920 -h 1080 -t texture.png
echo "===="
echo "2.|   $EXE --width 1920 --height 1080 --texture texture.png"
echo "===="
$EXE --width 1920 --height 1080 --texture texture.png
echo ""

echo "============================================================"
echo " -- TEST 11: Help + other flags --"
echo "===="
echo "1.|   $EXE --help -f model.obj -w 800"
echo "===="
$EXE --help -f model.obj -w 800
echo ""

echo "============================================================"
echo " -- ALL TESTS COMPLETED --"
echo "============================================================"
