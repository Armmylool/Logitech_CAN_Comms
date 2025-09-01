# --- Variables ---

# ชื่อโปรแกรมเป้าหมาย
TARGET = G29_Control.exe

# Compiler
CC = gcc

# Directories
SRC_DIR = ./src
INC_DIR = ./include
OBJ_DIR = obj

# ค้นหาไฟล์ .c ทั้งหมดใน SRC_DIR แบบอัตโนมัติ
SRCS = $(wildcard $(SRC_DIR)/*.c)

# สร้างรายชื่อไฟล์ .o จากไฟล์ .c โดยกำหนดให้ไปอยู่ที่ OBJ_DIR
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Compiler Flags
# -Wall  -> เปิดการแจ้งเตือนข้อผิดพลาดทั้งหมด
# -g     -> สร้างข้อมูลสำหรับ Debugging
CFLAGS = -Wall -g

# Include Paths
# -I$(INC_DIR) -> ทำให้ #include "G29.h" หาไฟล์เจอ
INC_PATHS = -I$(INC_DIR) -I"x86_64-w64-mingw32\include"

# Library Paths
LIB_PATHS = -L"x86_64-w64-mingw32\lib"

# Libraries to link
LIBS = -lmingw32 -lSDL2main -lSDL2 

# --- Rules ---

# .PHONY คือการบอกว่า 'all' และ 'clean' ไม่ใช่ชื่อไฟล์
.PHONY: all clean

# Rule แรกสุดที่จะทำงานเมื่อรัน 'make'
# โดยจะขึ้นอยู่กับการสร้าง OBJ_DIR และไฟล์ TARGET
all: $(OBJ_DIR) $(TARGET)

# Rule สำหรับการสร้างไฟล์โปรแกรม (Linking)
# $@ คือชื่อของ Target (ในที่นี้คือ $(TARGET))
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIB_PATHS) $(LIBS)
	-del /Q $(subst /,\,$(OBJ_DIR))
	-rmdir /Q $(subst /,\,$(OBJ_DIR))

# Rule สำหรับการสร้าง directory สำหรับเก็บไฟล์ .o
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# Rule สำหรับการคอมไพล์ไฟล์ .c จาก src ไปเป็น .o ใน obj
# $< คือชื่อของ Prerequisite ตัวแรก (ไฟล์ .c)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_PATHS) -c $< -o $@

# Rule สำหรับล้างไฟล์ที่ถูกสร้างขึ้น (object files และ executable)
clean:
	-del /Q $(subst /,\,$(OBJ_DIR))
	-rmdir /Q $(subst /,\,$(OBJ_DIR))
	-del $(TARGET)