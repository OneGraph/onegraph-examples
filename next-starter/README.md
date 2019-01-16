# OneGraph Next.js Starter

A Next.js starter kit including a full Apollo setup connecting OneGraph, react-onegraph for authentication and two simple example pages. 

## Running
Before running locally, we have to add a `env.js` file that exports our OneGraph API_ID. In order to get one, simply log in to your *OneGraph Dashboard* and choose an existing app or create a new one and copy its App Id in the *Settings* panel. 

##### env.js
```javascript
export const APP_ID = /* ID */
```

Now we can run the app using the following commands:
```sh
yarn
yarn dev
```
To run in production (for deployments), we have to run:
```sh
yarn build
yarn start
```

## Explore the source

#### Folders
- [integration](src/integration/): Apollo setup
- [pages](src/pages/): Pages for Next

----- 
#### Files
- [src/pages/_app.js](src/pages/_app.js): Passing our appId to the react-onegraph's AuthProvider
- [src/integration/apolloClient.js](src/integration/ApolloClient.js): Apollo setup including the authorization headers

#### Example Pages
- [src/pages/stocks.js](src/pages/stocks.js): A stock market query example
- [src/pages/twitter.js](src/pages/stocks.js): A Twitter example with authentication