copy_to = ../ru/LC_MESSAGES
all-local: guess.pot ru.mo
guess.pot: ../src/guess.c
	xgettext -k_ -c -o guess.pot ../src/guess.c
update-po: guess.pot
	msgmerge -U ru.po guess.pot
ru.mo: ru.po
	msgfmt -o ru.mo ru.po
install-data-local: ru.mo
	mkdir -p $(DESTDIR)$(localedir)/ru/LC_MESSAGES
	cp ru.mo $(DESTDIR)$(localedir)/ru/LC_MESSAGES/guess.mo
clean-local:
	rm -f *.mo guess.pot
