flappy:
	gcc -o flappy-bird flappy-bird.c

run: flappy
	./flappy-bird

clean:
	rm -rf flappy-bird
