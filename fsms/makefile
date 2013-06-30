REVISION=`date`

.PHONY: all
all: clean_tmp
	@echo "$(TEST_INFO) ($(shell echo $$HOSTNAME)) [$(REVISION)]"
	@echo "load average: $(shell cat /proc/loadavg)"
	@$(TIME) -o $(TMP_DIR).time -f "real: %es (%E)\nuser: %Us\nsys: %Ss\ncpu: %P\naverage memory usage: %KK\nmaximum resident set size: %MK\nnumber of times the process was swapped out of main memory: %W\nnumber of file system input: %I\nnumber of file system outputs: %O" make tests
	@echo -e "\nsummary:"
	@cat $(TMP_DIR).time | xargs -i% echo -e "\t%"
	@make results

include makefile.conf

.PHONY: test
test:
	@echo
	@echo "----------------------------------------------------------------"
	@echo "$(CXX) $(CXXFLAGS)"
	@echo "----------------------------------------------------------------"
	@mkdir -p "$(TMP_DIR)/$(CXX) $(CXXFLAGS)"
	@make $(TESTS)

test_%: clean
	@echo
	@echo "$@ ($(EVENT_CALLS) event calls)"
	@mkdir -p "$(TMP_DIR)/$(CXX) $(CXXFLAGS)/$@_$(EVENT_CALLS)"
	@make -s OUT="$(TMP_DIR)/$(CXX) $(CXXFLAGS)/$@_$(EVENT_CALLS)" TEST=$@ CXX=$(CXX) CXXFLAGS="$(CXXFLAGS) -DEVENT_CALLS=$(EVENT_CALLS)" $(FRAMEWORKS)

framework_%:
	@echo -e "\t$*"
	@cd $(shell echo $* | cut -d'.' -f1) && $(TIME) -o "$(OUT)/$*" -f "%es" make -s test_$(TEST) CXXFLAGS="$(CXXFLAGS) $(shell echo $* | grep "\." | cut -d'.' -f3 | xargs --no-run-if-empty -i% echo -D%) -DTEST_TYPE=$(shell echo $* | cut -d'.' -f2)"
	@cd $(shell echo $* | cut -d'.' -f1) && ls -lk $(TEST).exe | awk '{print $$5}' | xargs -i% echo %K >> "$(OUT)/$*"
	@cd $(shell echo $* | cut -d'.' -f1) && strip $(TEST).exe && ls -lk $(TEST).exe | awk '{print $$5}' | xargs -i% echo %K >> "$(OUT)/$*"
	@cd $(shell echo $* | cut -d'.' -f1) && $(TIME) -a -o "$(OUT)/$*" -f "%es" ./$(TEST).exe >/dev/null
	@cd $(shell echo $* | cut -d'.' -f1) && valgrind ./$(TEST).exe 2>&1 | sed "s/total heap usage/malloc\/free/g" | grep "malloc/free" | tail -1 | cut -d':' -f2 | awk '{printf "%s/%s (%sb)", $$1, $$3, $$5}' >> "$(OUT)/$*"

.PHONY: results
results:
	@echo
	@echo -n "generate results... "
	@cd ../doc && $(GENERATION_OPTIONS) scripts/gen_results.sh $(TMP_DIR) $(TEST_ASPECTS)
	@cd ../doc && make

.PHONY: clean
clean:
	@find . -iname "*.o" -or -iname "*.exe" | xargs --no-run-if-empty rm -f
	@cd ../doc && make clean_tmp

.PHONY: clean_tmp
clean_tmp:
	@rm -rf $(TMP_DIR)
	@rm -rf $(TMP_DIR).time

