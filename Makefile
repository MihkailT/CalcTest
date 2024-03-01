all:
	g++ -std=c++17 final.cpp calc.cpp -o final
	g++ -std=c++17 final_test.cpp calc.cpp -lcppunit -o final_test

test:
	chmod +x final
	./final_test

clean:
	$(RM) final
