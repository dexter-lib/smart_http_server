AUTO_TIMESTAMP := $(shell awk 'BEGIN {"g++ --version" | getline LINE; split(LINE, TK, "[. ]*"); GV=TK[3]TK[4]; if (GV<"42") print "-D __TIMESTAMPSTR__=\"" strftime() "\""; exit}')

PROG := smart_http_server

DEPS := . 


COMPILE_OPTIONS := \
	$(AUTO_TIMESTAMP) \
	-I . \
	-I ../  \
	-I ./utils \
	-I /root/boost_1_57_0 \
	-I /home/third-64/mysql/include \
	-I /home/third-64/libcurl/include \
	-I /home/third-64/openssl/include \
	-Wno-deprecated \
	-g -Wall -O0 -DNDEBUG

LINK_OPTIONS := \
    -L /usr/lib64 \
    -lpthread \
	-lrt \
	-lz \
    /root/boost_1_57_0/stage/lib/libboost_system.a \
    /root/boost_1_57_0/stage/lib/libboost_thread.a \
    /root/boost_1_57_0/stage/lib/libboost_program_options.a \
    /root/boost_1_57_0/stage/lib/libboost_date_time.a \
    /root/boost_1_57_0/stage/lib/libboost_filesystem.a \
	/home/dexter/music/work_push/third-64/openssl/lib/libssl.a \
	/home/dexter/music/work_push/third-64/openssl/lib/libcrypto.a \
	/home/dexter/music/work_push/third-64/libidn/lib/libidn.a 

	
SOURCE  := $(shell find $(DEPS) -name "*.cpp") $(shell find $(DEPS) -name "*.c")
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

CC			:= g++
CFLAGS		:= $(COMPILE_OPTIONS)
CPPFLAGS	:= $(CFLAGS)


$(PROG):	$(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(OBJS) $(LINK_OPTIONS) -o $@

clean:
	rm -rf $(PROG) $(OBJS) *~

install:
	cp $(PROG) /usr/local/smart_http_server/bin/

release:
	scp -P 12321 $(PROG) root@202.85.220.101:/usr/local/common_http_server/bin/$(PROG).latest

release2:
	scp -P 12321 $(PROG) root@118.26.224.142:/usr/local/common_http_server/bin/$(PROG).latest 
