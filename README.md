# OneGraph Examples

This repository contains several example applications leveraging the power of OneGraph.<br>
Each example is a self-contained package.<br>

## Examples

- [**Package Info**](https://packageinfo.now.sh): A service that provides detailed information about [npm packages](https://www.npmjs.com). It combines both information from [npm](https://www.npmjs.com) and [Github](http://github.com). ([source](packageinfo))
- [**Youtube Captions**](https://youtube-captions.now.sh): A service that lists interactive video captions for [YouTube](https://www.youtube.com/) videos. ([source](youtube-captions))

## Running locally

First of all, we have to add our own OneGraph App Id to the package.

```sh
echo env.js > export const APP_ID = /* YOUR APP ID HERE */
```

Now, if not explicitly stated differently, each example can be run by running the following 2 commands.

```sh
yarn
yarn dev
```
