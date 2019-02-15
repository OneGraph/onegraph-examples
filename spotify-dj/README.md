# SpotDJ

A serverless web app that enables anyone with a Spotify account to listen for someone else's Spotify music being played live.

It's built on top of Spotify using OneGraph for a serverless GraphQL API and for authentication.

Try it live at https://spotdj.onegraphapp.com

![Demo](demo.gif)

## Running SpotDJ Locally

Install dependencies:

```sh
git clone git@github.com:OneGraph/onegraph-examples.git
cd onegraph-examples/spotify-dj
yarn
# Send introspection query so Reason can give you automatic
# type check for all your queries using OneGraph.
yarn send-introspection-query https://serve.onegraph.com/dynamic?app_id=bafd4254-c229-48c2-8c53-44a01477a43e

```

Start the server:

```sh
yarn server

# in another tab
yarn start
```

## Running your own instance to avoid rate-limiting
By default, all queries are sharing a single Spotify OAuth client id, and will likely get rate limited at the most popular times. It's easy to set up your own for you and your friends though, and we really encourage you to try it out!

1. Simply create an app on [OneGraph](https://www.onegraph.com)
2. Visit the [Spotify Developer Portal](https://developer.spotify.com/dashboard/applications) and create a new Spotify app to get your own `client_id`/`client_secret`
3. Add your new Spotify OAuth client information into the OneGraph dashboard for you app under `Custom OAuth Credentials`
4. Add `http://localhost:8000` to the `CORS Origins` section

View the app at http://localhost:8000. Running in this environment provides hot reloading and support for routing; just edit and save the file and the browser will automatically refresh. 


## Built With
* [OneGraph](https://www.onegraph.com/) - Handle APIs and Authentication
* [PeerJS](https://peerjs.com/) - WebRTC
* [React-Motion](https://github.com/chenglou/react-motion) - UI Animation

## Contributors

- @yukims19: Check out the blog detailing how SpotDJ was designed, built, and pushed to production
- @xavxyz built and guided much of the initial animation for SpotDJ

## License
This project is licensed under the MIT License

