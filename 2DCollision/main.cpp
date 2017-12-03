#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h>
#include <NPC.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

void drawLine(const c2Ray& ray, RenderWindow& window);//Draws the Rays
//@Author Sean Nash De Andrade
//@login C00217019
//2D Collision Brief
//Known Issues: Player animation doesnt change as desired when making contact with shapes, Ray -> Circle Collision is barely working, Ray -> Poly Collision does not work


int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a NPC's sprites to display
	sf::Texture npc_texture;//All the textures used by the NPCs
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture player_texture;//All the textures used by the player
	if (!player_texture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Setup NPC's Default Animated Sprite
	AnimatedSprite npc_animated_sprite(npc_texture);//NPC AABB
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));


	//Capsule Sprite Set Up
	AnimatedSprite npc_animated_pillsprite(npc_texture);//NPC CAPSULE ANIMATION
	npc_animated_pillsprite.addFrame(sf::IntRect(3, 94, 84, 23));
	npc_animated_pillsprite.addFrame(sf::IntRect(88, 94, 84, 23));
	npc_animated_pillsprite.addFrame(sf::IntRect(173, 94, 84, 23));
	npc_animated_pillsprite.addFrame(sf::IntRect(258, 94, 84, 23));
	npc_animated_pillsprite.addFrame(sf::IntRect(343, 94, 84, 23));
	npc_animated_pillsprite.addFrame(sf::IntRect(428, 94, 84, 23));
	npc_animated_pillsprite.setPosition(sf::Vector2f(200, 40));


	//Polygon Sprite Set Up
	AnimatedSprite npc_animated_polysprite(npc_texture);//NPC POLYGON
	npc_animated_polysprite.addFrame(sf::IntRect(3, 175, 81, 80));
	npc_animated_polysprite.addFrame(sf::IntRect(88, 175, 81, 80));
	npc_animated_polysprite.addFrame(sf::IntRect(173, 175, 81, 80));
	npc_animated_polysprite.addFrame(sf::IntRect(258, 175, 81, 80));
	npc_animated_polysprite.addFrame(sf::IntRect(343, 175, 81, 80));
	npc_animated_polysprite.addFrame(sf::IntRect(428, 175, 81, 80));
	npc_animated_polysprite.setPosition(sf::Vector2f(200, 500));


	//Circle Sprite Set Up
	AnimatedSprite npc_animated_circlesprite(npc_texture);//NPC CIRCLE
	npc_animated_circlesprite.addFrame(sf::IntRect(3, 256, 81, 80));
	npc_animated_circlesprite.addFrame(sf::IntRect(88, 256, 81, 80));
	npc_animated_circlesprite.addFrame(sf::IntRect(173, 256, 81, 80));
	npc_animated_circlesprite.addFrame(sf::IntRect(258, 256, 81, 80));
	npc_animated_circlesprite.addFrame(sf::IntRect(343, 256, 81, 80));
	npc_animated_circlesprite.addFrame(sf::IntRect(428, 256, 81, 80));
	npc_animated_circlesprite.setPosition(sf::Vector2f(0, 200));

	// Setup Players Default Animated Sprite
	int playerMode = 0;//Tracks which shape the Player is currently
	AnimatedSprite player_animated_aabb_sprite(player_texture);
	player_animated_aabb_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	player_animated_aabb_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	player_animated_aabb_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	player_animated_aabb_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	player_animated_aabb_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	player_animated_aabb_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	AnimatedSprite player_animated_circle_sprite(player_texture);
	player_animated_circle_sprite.addFrame(sf::IntRect(3, 90, 82, 81));
	player_animated_circle_sprite.addFrame(sf::IntRect(88, 90, 82, 81));
	player_animated_circle_sprite.addFrame(sf::IntRect(173, 90, 82, 81));
	player_animated_circle_sprite.addFrame(sf::IntRect(258, 90, 82, 81));
	player_animated_circle_sprite.addFrame(sf::IntRect(343, 90, 82, 81));
	player_animated_circle_sprite.addFrame(sf::IntRect(428, 90, 82, 81));

	// Setup the NPCs
	GameObject &npc = NPC(npc_animated_sprite);
	GameObject &capsule = NPC(npc_animated_pillsprite);
	GameObject &polygon = NPC(npc_animated_polysprite);
	GameObject &circle = NPC(npc_animated_circlesprite);



	// Setup the Player
	GameObject &playerAABB = Player(player_animated_aabb_sprite);
	GameObject &playerCircle = Player(player_animated_circle_sprite);

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(playerAABB.getAnimatedSprite().getPosition().x, playerAABB.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(playerAABB.getAnimatedSprite().getGlobalBounds().width / 6, playerAABB.getAnimatedSprite().getGlobalBounds().width / 6);


	//Setup Player Circle
	c2Circle circle_player;
	circle_player.p = c2V(playerCircle.getAnimatedSprite().getPosition().x + playerCircle.getAnimatedSprite().getGlobalBounds().width / 2, playerCircle.getAnimatedSprite().getPosition().y + playerCircle.getAnimatedSprite().getGlobalBounds().height / 2);
	circle_player.r = playerCircle.getAnimatedSprite().getGlobalBounds().height / 2;

	//Set Up Player ray
	c2Ray ray_player;
	ray_player.p = c2V(700, 600);
	ray_player.d = c2Norm(c2V(1, -1));
	ray_player.t = 100;


	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width,
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);

	//Setup NPC Capsule
	c2Capsule capsule_npc;
	capsule_npc.a = c2V(capsule.getAnimatedSprite().getPosition().x + capsule.getAnimatedSprite().getGlobalBounds().height / 2, capsule.getAnimatedSprite().getPosition().y + capsule.getAnimatedSprite().getGlobalBounds().height / 2);
	capsule_npc.b = c2V(capsule.getAnimatedSprite().getPosition().x + capsule.getAnimatedSprite().getGlobalBounds().width - capsule.getAnimatedSprite().getGlobalBounds().height / 2, capsule.getAnimatedSprite().getPosition().y + capsule.getAnimatedSprite().getGlobalBounds().height / 2);
	capsule_npc.r = capsule.getAnimatedSprite().getGlobalBounds().height / 2;

	//Setup NPC Polygon
	c2Poly polygon_npc;
	polygon_npc.count = 4;
	polygon_npc.verts[0] = c2V(polygon.getAnimatedSprite().getPosition().x, polygon.getAnimatedSprite().getPosition().y);
	polygon_npc.verts[1] = c2V(polygon.getAnimatedSprite().getPosition().x + polygon.getAnimatedSprite().getGlobalBounds().width, polygon.getAnimatedSprite().getPosition().y);
	polygon_npc.verts[2] = c2V(polygon.getAnimatedSprite().getPosition().x + polygon.getAnimatedSprite().getGlobalBounds().width, polygon.getAnimatedSprite().getPosition().y + polygon.getAnimatedSprite().getGlobalBounds().height);
	polygon_npc.verts[3] = c2V(polygon.getAnimatedSprite().getPosition().x , polygon.getAnimatedSprite().getPosition().y + polygon.getAnimatedSprite().getGlobalBounds().height );

	//Setup NPC Circle
	c2Circle circle_npc;
	circle_npc.p = c2V(circle.getAnimatedSprite().getPosition().x + circle.getAnimatedSprite().getGlobalBounds().width/2, circle.getAnimatedSprite().getPosition().y + +circle.getAnimatedSprite().getGlobalBounds().height / 2);
	circle_npc.r = circle.getAnimatedSprite().getGlobalBounds().height / 2;

	//Set Up NPC ray
	c2Ray ray_npc;
	ray_npc.p = c2V(700, 600);
	ray_npc.d = c2Norm(c2V(0, -1));
	ray_npc.t = 100;

	// Initialize Input
	Input input;

	// Collision result
	int result = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.1f, 0.2f);
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		playerAABB.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		playerCircle.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		ray_player.p = c2V(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x, window.mapPixelToCoords(sf::Mouse::getPosition(window)).y);
		// Move The NPC
		sf::Vector2f move_to(npc.getAnimatedSprite().getPosition().x + direction.x, npc.getAnimatedSprite().getPosition().y + direction.y);

		if (move_to.x < 0) {
			direction.x *= -1;
			move_to.x = 0 + npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.x + npc.getAnimatedSprite().getGlobalBounds().width >= 800) { 
			direction.x *= -1;
			move_to.x = 800 - npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.y < 0) { 
			direction.y *= -1;
			move_to.y = 0 + npc.getAnimatedSprite().getGlobalBounds().height;
		}
		else if (move_to.y + npc.getAnimatedSprite().getGlobalBounds().height >= 600) {
			direction.y *= -1;
			move_to.y = 600 - npc.getAnimatedSprite().getGlobalBounds().height;
		}
		
		npc.getAnimatedSprite().setPosition(move_to);

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(
			npc.getAnimatedSprite().getPosition().x, 
			npc.getAnimatedSprite().getPosition().y
		);

		aabb_npc.max = c2V(
			npc.getAnimatedSprite().getPosition().x +
			npc.getAnimatedSprite().getGlobalBounds().width,
			npc.getAnimatedSprite().getPosition().y +
			npc.getAnimatedSprite().getGlobalBounds().height
		);

		// Update Player AABB
		aabb_player.min = c2V(
			playerAABB.getAnimatedSprite().getPosition().x, 
			playerAABB.getAnimatedSprite().getPosition().y
		);
		aabb_player.max = c2V(
			playerAABB.getAnimatedSprite().getPosition().x +
			playerAABB.getAnimatedSprite().getGlobalBounds().width, 
			playerAABB.getAnimatedSprite().getPosition().y + 
			playerAABB.getAnimatedSprite().getGlobalBounds().height
		);
		circle_player.p = c2V(playerCircle.getAnimatedSprite().getPosition().x +
			playerCircle.getAnimatedSprite().getGlobalBounds().width / 2,
			playerCircle.getAnimatedSprite().getPosition().y +
			playerCircle.getAnimatedSprite().getGlobalBounds().height / 2);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))//Changes Player modes
				{
					playerMode++;
					if (playerMode > 2)//Resets Player Mode
					{
						playerMode = 0;
					}
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))//Changes Player modes
				{
					playerMode--;
					if (playerMode < 0)//Resets Player Mode
					{
						playerMode = 2;
					}
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		playerAABB.handleInput(input);

		// Update the Player
		playerAABB.update();
		playerCircle.update();


		// Update the Player
		npc.update();

		capsule.update();
		circle.update();
		polygon.update();

		// Check for collisions
		if (playerMode == 0)//AABB Collisions (AABB -> AABB,Capsule,Poly,Ray
		{
			result = c2AABBtoAABB(aabb_player, aabb_npc);//Square to Square
			cout << ((result != 0) ? ("AABB -> AABB Collision") : "") << endl;
			if (result) {
				playerAABB.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerAABB.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			result = c2CircletoAABB(circle_npc, aabb_player);//Square to Circle
			cout << ((result != 0) ? ("AABB -> Circle Collision") : "") << endl;
			if (result) {
				playerAABB.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerAABB.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}


			result = c2AABBtoCapsule(aabb_player, capsule_npc);//Square to Capsule
			cout << ((result != 0) ? ("AABB -> Capsule Collision") : "") << endl;
			if (result) {
				playerAABB.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerAABB.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}
			result = c2AABBtoPoly(aabb_player, &polygon_npc, 0);//Square to Poly
			cout << ((result != 0) ? ("AABB -> Poly Collision") : "") << endl;
			if (result) {
				playerAABB.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerAABB.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}

			c2Raycast rayCast;
			result = c2RaytoAABB(ray_npc, aabb_player, &rayCast);//Square to Ray
			cout << ((result != 0) ? ("AABB -> Ray Collision") : "") << endl;
			if (result) {
				playerAABB.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerAABB.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			}
		}
		if (playerMode == 1)//Circle Collisions (Circle -> AABB,Circle,Ray,Capsule, Poly)
		{
			result = c2CircletoAABB(circle_player, aabb_npc);//Circle to Square
			cout << ((result != 0) ? ("Circle -> AABB Collision") : "") << endl;
			if (result) {
				playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			};

			result = c2CircletoCircle(circle_player, circle_npc);//Circle to Circle
			cout << ((result != 0) ? ("Circle -> Circle Collision") : "") << endl;
			if (result) {
					playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
				}
				else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			};

			c2Raycast rayCast;
			result = c2RaytoCircle(ray_npc, circle_player, &rayCast);//Circle to Ray
			cout << ((result != 0) ? ("Circle -> Ray Collision") : "") << endl;
			if (result) {
				playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			}
			else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			};

			result = c2CircletoCapsule(circle_player, capsule_npc);//Circle to Capsule
			cout << ((result != 0) ? ("Circle -> Capsule Collision") : "") << endl;
			if (result) {
					playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
				}
				else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			};

			result = c2CircletoPoly(circle_player, &polygon_npc,0);//Circle to Poly
			cout << ((result != 0) ? ("Circle -> Poly Collision") : "") << endl;
			if (result) {
					playerCircle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
				}
				else {
				playerCircle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			};

		}
		if (playerMode == 2)//Ray Collisions (Ray -> AABB, Capsule,Circle,Poly)
		{
			c2Raycast rayCastAABB;
			result = c2RaytoAABB(ray_player, aabb_npc, &rayCastAABB);//Ray to Square
			cout << ((result != 0) ? ("Ray -> AABB Collision") : "") << endl;
			if (result) {
				//Cant think of what to put in here for rays.
			}

			c2Raycast rayCastCapsule;
			result = c2RaytoCapsule(ray_player, capsule_npc, &rayCastCapsule);//Ray to Capsule
			cout << ((result != 0) ? ("Ray -> Capsule Collision") : "") << endl;
			if (result) {
				//Cant think of what to put in here for rays.
			}

			c2Raycast rayCastCircle;
			result = c2RaytoCircle(ray_player, circle_npc, &rayCastCircle);//Ray to Circle && ISSUES RETURN HERE
			cout << ((result != 0) ? ("Ray -> Circle Collision") : "") << endl;
			if (result) {
				//Cant think of what to put in here for rays.
			}

			c2Raycast rayCastPoly;
			result = c2RaytoPoly(ray_player, &polygon_npc,0, &rayCastPoly);//Ray to Poly && ISSUES RETURN HERE
			cout << ((result != 0) ? ("Ray -> Poly Collision") : "") << endl;
			if (result) {
				//Cant think of what to put in here for rays.
			}




		}

		// Clear screen
		window.clear();


		// Draw the NPC's Current Animated Sprite
		window.draw(npc.getAnimatedSprite());

		window.draw(capsule.getAnimatedSprite());
		window.draw(polygon.getAnimatedSprite());
		window.draw(circle.getAnimatedSprite());

		drawLine(ray_npc,window);

				// Draw the Players Current Animated Sprite
		if (playerMode == 0)
		{
			window.draw(playerAABB.getAnimatedSprite());
		}
		if (playerMode == 1)
		{
			window.draw(playerCircle.getAnimatedSprite());
		}
		if (playerMode == 2)
		{
			drawLine(ray_player, window);
		}

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};

void drawLine(const c2Ray & npc_ray, RenderWindow & window)
{
	sf::Vertex shape[] = {
		sf::Vertex(sf::Vector2f(npc_ray.p.x, npc_ray.p.y), sf::Color::White),
		sf::Vertex(sf::Vector2f(npc_ray.p.x + (npc_ray.d.x * npc_ray.t), npc_ray.p.y + (npc_ray.d.y * npc_ray.t)), sf::Color::White)
	};

	window.draw(shape, 2, sf::Lines);

}
