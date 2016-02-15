SERV_SRC=$(PWD)/Server/
CLIENT_SRC=$(PWD)/Client/
BUILD=$(PWD)/Build/


all: $(BUILD)Server $(BUILD)Client $(BUILD)Filler



#############################################################################
##################### ID_PASS file filler ###################################
#############################################################################

$(BUILD)Filler: $(PWD)/fill_file.c
	-echo "#define FILE_PATH \"$(PWD)/file.txt\"" > $(PWD)/path.h
	gcc -I $(PWD) -o $(BUILD)Filler $(PWD)/fill_file.c

#############################################################################
##################### Server ################################################
#############################################################################

$(BUILD)Server: $(SERV_SRC)server.o $(SERV_SRC)main.o
	-mkdir $(BUILD)
	gcc -o $(BUILD)Server $(SERV_SRC)server.o $(SERV_SRC)main.o -pthread

$(SERV_SRC)server.o: $(SERV_SRC)server.h $(SERV_SRC)server.c $(SERV_SRC)main.c defs.h
	-echo "#define FILE_PATH \"$(PWD)/file.txt\"" > $(PWD)/path.h
	gcc -c $(SERV_SRC)server.c -I $(PWD) -o $(SERV_SRC)server.o -pthread
$(SERV_SRC)main.o: $(SERV_SRC)server.h $(SERV_SRC)server.c $(SERV_SRC)main.c defs.h
	-echo "#define FILE_PATH \"$(PWD)/file.txt\"" > $(PWD)/path.h
	gcc -c $(SERV_SRC)main.c -I $(PWD)	-o $(SERV_SRC)main.o -pthread

#############################################################################
###################### Client ###############################################
#############################################################################

$(BUILD)Client: $(CLIENT_SRC)main.o $(CLIENT_SRC)client.o
	-mkdir $(BUILD)
	gcc -o $(BUILD)Client $(CLIENT_SRC)client.o $(CLIENT_SRC)main.o -pthread

$(CLIENT_SRC)client.o: $(CLIENT_SRC)client.c defs.h
	-echo "#define FILE_PATH \"$(PWD)/file.txt\"" > $(PWD)/path.h
	gcc -c $(CLIENT_SRC)client.c -I $(PWD) -o $(CLIENT_SRC)client.o -pthread

$(CLIENT_SRC)main.o: $(CLIENT_SRC)main.c defs.h
	-echo "#define FILE_PATH \"$(PWD)/file.txt\"" > $(PWD)/path.h
	gcc -c $(CLIENT_SRC)main.c -I $(PWD) -o $(CLIENT_SRC)main.o -pthread




clean:
	-rm $(CLIENT_SRC)main.o
	-rm $(CLIENT_SRC)client.o
	-rm $(SERV_SRC)main.o
	-rm $(SERV_SRC)server.o
	-rm $(BUILD)Client
	-rm $(BUILD)Server
	-rm $(BUILD)Filler
	-rm $(PWD)/file.txt

  
	

