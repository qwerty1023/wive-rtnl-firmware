#Makefile for GCC
#
#Tom St Denis

#version of library 
VERSION=0.40


srcdir=.

# So that libtommath can include Dropbear headers for options and m_burn()
CFLAGS += -I$(srcdir)/../libtomcrypt/src/headers/ -I$(srcdir)/../

ifndef IGNORE_SPEED

#for speed 
#CFLAGS += -O3 -funroll-all-loops

#for size 
#CFLAGS += -Os

#x86 optimizations [should be valid for any GCC install though]
#CFLAGS  += -fomit-frame-pointer

#debug
#CFLAGS += -g3

endif

#install as this user
ifndef INSTALL_GROUP
   GROUP=wheel
else
   GROUP=$(INSTALL_GROUP)
endif

ifndef INSTALL_USER
   USER=root
else
   USER=$(INSTALL_USER)
endif

#default files to install
ifndef LIBNAME
   LIBNAME=libtommath.a
endif

default: ${LIBNAME}

HEADERS=tommath.h tommath_class.h tommath_superclass.h

#LIBPATH-The directory for libtommath to be installed to.
#INCPATH-The directory to install the header files for libtommath.
#DATAPATH-The directory to install the pdf docs.
DESTDIR=
LIBPATH=/usr/lib
INCPATH=/usr/include
DATAPATH=/usr/share/doc/libtommath/pdf

OBJECTS=bncore.o bn_mp_init.o bn_mp_clear.o bn_mp_exch.o bn_mp_grow.o bn_mp_shrink.o \
bn_mp_clamp.o bn_mp_zero.o  bn_mp_set.o bn_mp_set_int.o bn_mp_init_size.o bn_mp_copy.o \
bn_mp_init_copy.o bn_mp_abs.o bn_mp_neg.o bn_mp_cmp_mag.o bn_mp_cmp.o bn_mp_cmp_d.o \
bn_mp_rshd.o bn_mp_lshd.o bn_mp_mod_2d.o bn_mp_div_2d.o bn_mp_mul_2d.o bn_mp_div_2.o \
bn_mp_mul_2.o bn_s_mp_add.o bn_s_mp_sub.o bn_fast_s_mp_mul_digs.o bn_s_mp_mul_digs.o \
bn_fast_s_mp_mul_high_digs.o bn_s_mp_mul_high_digs.o bn_fast_s_mp_sqr.o bn_s_mp_sqr.o \
bn_mp_add.o bn_mp_sub.o bn_mp_karatsuba_mul.o bn_mp_mul.o bn_mp_karatsuba_sqr.o \
bn_mp_sqr.o bn_mp_div.o bn_mp_mod.o bn_mp_add_d.o bn_mp_sub_d.o bn_mp_mul_d.o \
bn_mp_div_d.o bn_mp_mod_d.o bn_mp_expt_d.o bn_mp_addmod.o bn_mp_submod.o \
bn_mp_mulmod.o bn_mp_sqrmod.o bn_mp_gcd.o bn_mp_lcm.o bn_fast_mp_invmod.o bn_mp_invmod.o \
bn_mp_reduce.o bn_mp_montgomery_setup.o bn_fast_mp_montgomery_reduce.o bn_mp_montgomery_reduce.o \
bn_mp_exptmod_fast.o bn_mp_exptmod.o bn_mp_2expt.o bn_mp_n_root.o bn_mp_jacobi.o bn_reverse.o \
bn_mp_count_bits.o bn_mp_read_unsigned_bin.o bn_mp_read_signed_bin.o bn_mp_to_unsigned_bin.o \
bn_mp_to_signed_bin.o bn_mp_unsigned_bin_size.o bn_mp_signed_bin_size.o  \
bn_mp_xor.o bn_mp_and.o bn_mp_or.o bn_mp_rand.o bn_mp_montgomery_calc_normalization.o \
bn_mp_prime_is_divisible.o bn_prime_tab.o bn_mp_prime_fermat.o bn_mp_prime_miller_rabin.o \
bn_mp_prime_is_prime.o bn_mp_prime_next_prime.o bn_mp_dr_reduce.o \
bn_mp_dr_is_modulus.o bn_mp_dr_setup.o bn_mp_reduce_setup.o \
bn_mp_toom_mul.o bn_mp_toom_sqr.o bn_mp_div_3.o bn_s_mp_exptmod.o \
bn_mp_reduce_2k.o bn_mp_reduce_is_2k.o bn_mp_reduce_2k_setup.o \
bn_mp_reduce_2k_l.o bn_mp_reduce_is_2k_l.o bn_mp_reduce_2k_setup_l.o \
bn_mp_radix_smap.o bn_mp_read_radix.o bn_mp_toradix.o bn_mp_radix_size.o \
bn_mp_fread.o bn_mp_fwrite.o bn_mp_cnt_lsb.o bn_error.o \
bn_mp_init_multi.o bn_mp_clear_multi.o bn_mp_exteuclid.o bn_mp_toradix_n.o \
bn_mp_prime_random_ex.o bn_mp_get_int.o bn_mp_sqrt.o bn_mp_is_square.o bn_mp_init_set.o \
bn_mp_init_set_int.o bn_mp_invmod_slow.o bn_mp_prime_rabin_miller_trials.o \
bn_mp_to_signed_bin_n.o bn_mp_to_unsigned_bin_n.o

$(LIBNAME):  $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)
	$(RANLIB) $@

#make a profiled library (takes a while!!!)
#
# This will build the library with profile generation
# then run the test demo and rebuild the library.
# 
# So far I've seen improvements in the MP math
profiled:
	make CFLAGS="$(CFLAGS) -fprofile-arcs -DTESTING" timing
	./ltmtest
	rm -f *.a *.o ltmtest
	make CFLAGS="$(CFLAGS) -fbranch-probabilities"

#make a single object profiled library 
profiled_single:
	perl gen.pl
	$(CC) $(CFLAGS) -fprofile-arcs -DTESTING -c mpi.c -o mpi.o
	$(CC) $(CFLAGS) -DTESTING -DTIMER demo/timing.c mpi.o -o ltmtest
	./ltmtest
	rm -f *.o ltmtest
	$(CC) $(CFLAGS) -fbranch-probabilities -DTESTING -c mpi.c -o mpi.o
	$(AR) $(ARFLAGS) $(LIBNAME) mpi.o
	$(RANLIB) $(LIBNAME)	

install: $(LIBNAME)
	install -d -g $(GROUP) -o $(USER) $(DESTDIR)$(LIBPATH)
	install -d -g $(GROUP) -o $(USER) $(DESTDIR)$(INCPATH)
	install -g $(GROUP) -o $(USER) $(LIBNAME) $(DESTDIR)$(LIBPATH)
	install -g $(GROUP) -o $(USER) $(HEADERS) $(DESTDIR)$(INCPATH)

test: $(LIBNAME) demo/demo.o
	$(CC) $(CFLAGS) demo/demo.o $(LIBNAME) -o test
	
mtest: test	
	cd mtest ; $(CC) $(CFLAGS) mtest.c -o mtest
        
timing: $(LIBNAME)
	$(CC) $(CFLAGS) -DTIMER demo/timing.c $(LIBNAME) -o ltmtest

# makes the LTM book DVI file, requires tetex, perl and makeindex [part of tetex I think]
docdvi: tommath.src
	cd pics ; MAKE=${MAKE} ${MAKE} 
	echo "hello" > tommath.ind
	perl booker.pl
	latex tommath > /dev/null
	latex tommath > /dev/null
	makeindex tommath
	latex tommath > /dev/null

# poster, makes the single page PDF poster
poster: poster.tex
	pdflatex poster
	rm -f poster.aux poster.log 

# makes the LTM book PDF file, requires tetex, cleans up the LaTeX temp files
docs:   docdvi
	dvipdf tommath
	rm -f tommath.log tommath.aux tommath.dvi tommath.idx tommath.toc tommath.lof tommath.ind tommath.ilg
	cd pics ; MAKE=${MAKE} ${MAKE} clean
	
#LTM user manual
mandvi: bn.tex
	echo "hello" > bn.ind
	latex bn > /dev/null
	latex bn > /dev/null
	makeindex bn
	latex bn > /dev/null

#LTM user manual [pdf]
manual:	mandvi
	pdflatex bn >/dev/null
	rm -f bn.aux bn.dvi bn.log bn.idx bn.lof bn.out bn.toc

pretty: 
	perl pretty.build

clean:
	rm -f *.bat *.pdf *.o *.a *.obj *.lib *.exe *.dll etclib/*.o demo/demo.o test ltmtest mpitest mtest/mtest mtest/mtest.exe \
        *.idx *.toc *.log *.aux *.dvi *.lof *.ind *.ilg *.ps *.log *.s mpi.c *.da *.dyn *.dpi tommath.tex `find . -type f | grep [~] | xargs` *.lo *.la
	rm -rf .libs
	-cd etc && MAKE=${MAKE} ${MAKE} clean
	-cd pics && MAKE=${MAKE} ${MAKE} clean

#zipup the project (take that!)
no_oops: clean
	cd .. ; cvs commit 
	echo Scanning for scratch/dirty files
	find . -type f | grep -v CVS | xargs -n 1 bash mess.sh

zipup: clean manual poster docs
	perl gen.pl ; mv mpi.c pre_gen/ ; \
	cd .. ; rm -rf ltm* libtommath-$(VERSION) ; mkdir libtommath-$(VERSION) ; \
	cp -Ra ./libtommath/* ./libtommath-$(VERSION)/ ; \
	tar -c libtommath-$(VERSION)/* | bzip2 -9vvc > ltm-$(VERSION).tar.bz2 ; \
	zip -9 -r ltm-$(VERSION).zip libtommath-$(VERSION)/* ; \
	mv -f ltm* ~ ; rm -rf libtommath-$(VERSION)
