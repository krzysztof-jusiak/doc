.SILENT:
XPDF=xpdf
BUILD=pdflatex -file-line-error -interaction=nonstopmode -jobname=$(TARGET) $(TARGET)

all:
	@make TARGET=fsms pdf
	@make TARGET=fsms_results pdf

pdf: build_1 build_2
	@if [ `ps aux | grep $(XPDF) | grep -v grep | grep -c $(TARGET)` -eq 0 ]; then $(XPDF) -remote $(TARGET) $(TARGET).pdf & else $(XPDF) -remote $(TARGET) -reload; fi
	@echo $(PWD)/$(TARGET).pdf

build_1:
	@$(BUILD) | grep -P ':\d{1,5}:'; exit $$PIPESTATUS

build_2:
	@$(BUILD) | grep -P ':\d{1,5}:|LaTeX Warning'; exit $$PIPESTATUS

clean_tmp:
	@rm -f *.aux *.dvi *.log *.out *.lof *.lot *.toc

clean: clean_tmp
	 rm -f *.pdf

