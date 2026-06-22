import pygame
from random import randint

pygame.init()
pygame.display.set_caption('FlappyBird')
gameIcon = pygame.image.load(r'assets\images\title.png')
pygame.display.set_icon(gameIcon)

f = open(r'assets\highScore.txt')
highScore = int(f.read())
f.close()

def play():
    running = True
    clock = pygame.time.Clock()
    BLACK = (0,0,0)
    RED = (255,0,0)
    screen = pygame.display.set_mode((900,504))
    font1 = pygame.font.SysFont('sans', 50)
    font2 = pygame.font.SysFont('sans', 20)
    background = pygame.image.load(r'assets\images\background.png')
    pipe = pygame.image.load(r'assets\images\pipe.png')
    pipeInverse = pygame.transform.flip(pipe, False, True)
    bird = pygame.image.load(r'assets\images\bird.png')
    message = pygame.image.load(r'assets\images\message.png')
    gameover = pygame.image.load(r'assets\images\gameover.png')
    die = pygame.mixer.Sound(r'assets\sounds\sfx_die.wav')
    point = pygame.mixer.Sound(r'assets\sounds\sfx_point.wav')
    wing = pygame.mixer.Sound(r'assets\sounds\sfx_wing.wav')
    quit_ = False
    global highScore

    while running:
        clock.tick(60)
        screen.fill(BLACK)
        screen.blit(background, (0,0))
        textMessage = font1.render('Press Space to play', True, BLACK)
        textHighScore = font2.render('High Score: '+str(highScore),True,BLACK)
        screen.blit(message, (230,100))
        screen.blit(textMessage, (260,350))
        screen.blit(textHighScore, (0,0))

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                quit_ = True 
                running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                running = False

        pygame.display.update()

    if not quit_:
        running = True
    
    pipeGap = 140 
    pipeWidth = 50
    pipeDistance = 150
    pipeHeight = 309
    pipe1x, pipe2x, pipe3x, pipe4x, pipe5x = 900, 1100, 1300, 1500, 1700
    pipe1height, pipe2height, pipe3height, pipe4height, pipe5height = randint(100,269), randint(100,269), randint(100,269), randint(100,269), randint(100,269)
    pipeVelocity = 3
    totalPipe = 5
    birdX = 75
    birdY = 200
    birdDropVelocity = 0
    gravity = 0.35
    score = 0
    pipe1pass, pipe2pass, pipe3pass, pipe4pass, pipe5pass = False, False, False, False, False 
    diePass = False

    while running:
        clock.tick(60)
        screen.fill(BLACK)
        # draw background
        screen.blit(background, (0,0))
        # draw pipes
        pipe1x -= pipeVelocity
        pipe2x -= pipeVelocity
        pipe3x -= pipeVelocity
        pipe4x -= pipeVelocity
        pipe5x -= pipeVelocity
        pipeInverse1rect = screen.blit(pipeInverse, (pipe1x,pipe1height-pipeHeight))
        pipeInverse2rect = screen.blit(pipeInverse, (pipe2x,pipe2height-pipeHeight))
        pipeInverse3rect = screen.blit(pipeInverse, (pipe3x,pipe3height-pipeHeight))
        pipeInverse4rect = screen.blit(pipeInverse, (pipe4x,pipe4height-pipeHeight))
        pipeInverse5rect = screen.blit(pipeInverse, (pipe5x,pipe5height-pipeHeight))
        pipe1rect = screen.blit(pipe, (pipe1x,pipe1height+pipeGap))
        pipe2rect = screen.blit(pipe, (pipe2x,pipe2height+pipeGap))
        pipe3rect = screen.blit(pipe, (pipe3x,pipe3height+pipeGap))
        pipe4rect = screen.blit(pipe, (pipe4x,pipe4height+pipeGap))
        pipe5rect = screen.blit(pipe, (pipe5x,pipe5height+pipeGap))
        # generate new pipes 
        if pipe1x<-pipeWidth: 
            pipe1x=(pipeWidth+pipeDistance)*totalPipe-pipeWidth
            pipe1height=randint(104,300)
            pipe1pass=False
        if pipe2x<-pipeWidth: 
            pipe2x=(pipeWidth+pipeDistance)*totalPipe-pipeWidth
            pipe2height=randint(104,300)
            pipe2pass=False
        if pipe3x<-pipeWidth: 
            pipe3x=(pipeWidth+pipeDistance)*totalPipe-pipeWidth
            pipe3height=randint(104,300)
            pipe3pass=False
        if pipe4x<-pipeWidth: 
            pipe4x=(pipeWidth+pipeDistance)*totalPipe-pipeWidth
            pipe4height=randint(104,300)
            pipe4pass=False
        if pipe5x<-pipeWidth: 
            pipe5x=(pipeWidth+pipeDistance)*totalPipe-pipeWidth
            pipe5height=randint(104,300)
            pipe5pass=False
        # draw bird   
        birdRect = screen.blit(bird, (birdX,birdY))
        # bird fall and jump
        birdY+=birdDropVelocity
        birdDropVelocity+=gravity
        # update score
        if pipe1x+pipeWidth<=birdX and not pipe1pass:
            score += 1
            pipe1pass = True
            pygame.mixer.Sound.play(point)
            pygame.mixer.music.stop()
        if pipe2x+pipeWidth<=birdX and not pipe2pass:
            score += 1
            pipe2pass = True
            pygame.mixer.Sound.play(point)
            pygame.mixer.music.stop()
        if pipe3x+pipeWidth<=birdX and not pipe3pass:
            score += 1
            pipe3pass = True
            pygame.mixer.Sound.play(point)
            pygame.mixer.music.stop()
        if pipe4x+pipeWidth<=birdX and not pipe4pass:
            score += 1
            pipe4pass = True
            pygame.mixer.Sound.play(point)
            pygame.mixer.music.stop()
        if pipe5x+pipeWidth<=birdX and not pipe5pass:
            score += 1
            pipe5pass = True
            pygame.mixer.Sound.play(point)
            pygame.mixer.music.stop()
        # print score
        textScore = font1.render(str(score), True, RED)
        screen.blit(textScore, (430,0))
        # check lose
        for i in [pipeInverse1rect,pipeInverse2rect,pipeInverse3rect,pipeInverse4rect,pipeInverse5rect,pipe1rect,pipe2rect,pipe3rect,pipe4rect,pipe5rect]:
            if birdRect.colliderect(i) or birdY<0 or birdY>509:
                birdDropVelocity = 0
                pipeVelocity = 0
                screen.blit(gameover,(100,100))
                textGameover = font1.render('Click to back to Home', True, BLACK)
                screen.blit(textGameover, (250,350))
                if highScore < score: highScore = score
                f = open(r'assets\highScore.txt', 'w')
                f.write(str(highScore))
                f.close()
                if not diePass:
                    pygame.mixer.Sound.play(die)
                    pygame.mixer.music.stop()
                    diePass = True
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        running = False
                    if event.type == pygame.MOUSEBUTTONDOWN:
                        play()
                        return
        # check mouse and keyboard
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            if event.type==pygame.KEYDOWN and event.key==pygame.K_SPACE:
                birdDropVelocity = -6
                pygame.mixer.Sound.play(wing)
                pygame.mixer.music.stop()

        pygame.display.update()

play()
pygame.quit()