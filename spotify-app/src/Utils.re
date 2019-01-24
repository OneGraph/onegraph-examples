/* require css file for side effect only */
[@bs.val] external requireCSS: string => unit = "require";

/* require an asset (eg. an image) and return exported string value (image URI) */
[@bs.val] external requireAssetURI: string => string = "require";

let getImageUrl = (~images, ~defaultImage) => {
  let urls =
    Js.Array.map(
      image =>
        switch (image##url) {
        | Some(url) => url
        | None => ""
        },
      images,
    )
    |> Js.Array.filter(url => url !== "");

  Array.length(urls) > 1
    ? urls[1]
    : Array.length(urls) == 1
      ? urls[0]
      : defaultImage;
}
