CC=gcc
AFLAGS=-Wall -g
LDFLAGS= -lmysqlclient
OBJS= add.cgi del.cgi mod.cgi sel.cgi fdel.cgi suse.cgi tuser.cgi addc.cgi selc.cgi score.cgi tsel.cgi selscore.cgi chosecourse.cgi

all:$(OBJS)

$(OBJS):%.cgi:%.c
	$(CC) $(AFLAGS) $< cgic.c -o $@ $(LDFLAGS)

.PHONY:clean
clean:
	rm ./*.cgi

install:
	cp *.cgi /usr/lib/cgi-bin/sx/
	cp head.html footer.html sindex.html tindex.html /usr/lib/cgi-bin/sx/
