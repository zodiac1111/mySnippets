clean:
	rm -rf main-old-need-to-patch patch.patch

# make patch
makepatch:
	diff main-old main-changed  -uNr  > patch.patch

# use patch
cp-old-dir:
	cp -r main-old main-old-need-to-patch 
usepatch:cp-old-dir
	cd main-old-need-to-patch && patch -p1 -E < ../patch.patch