export default function preparePackageData({
  repository,
  time,
  homepage,
  name,
  description,
  license,
  bundlephobia,
  downloads,
  readme,
  versions,
  keywords = [],
  maintainers = [],
}) {
  const sortedVersions = time.versions.sort(
    (a, b) => new Date(a.date).getTime() < new Date(b.date).getTime() || -1
  )

  const lastPublish = sortedVersions[0]
  const currentVersion = lastPublish.version

  const lastPublishDate = new Date(lastPublish.date)
  const perDayDownloads = downloads.period.perDay

  let dependencies = versions.find(
    version => version.version === currentVersion
  ).dependencies

  if (bundlephobia && bundlephobia.dependencySizes) {
    dependencies = dependencies.map(dependency => {
      const bundlephobiaDependency = bundlephobia.dependencySizes.find(
        pkg => pkg.name === dependency.name
      )

      if (bundlephobiaDependency) {
        return {
          ...dependency,
          bundlephobia: bundlephobiaDependency,
        }
      }

      return dependency
    })
  }

  const data = {
    currentVersion,
    dependencies,
    bundlephobia,
    bundlephobiaHistory: bundlephobia ? bundlephobia.history : [],
    lastPublishDate,
    versions: sortedVersions,
    homepage,
    name,
    description,
    primaryLanguage: {},
    downloads: perDayDownloads,
    maintainers,
    keywords: keywords || [],
    license: license.type,
    readme,
  }

  const githubRepository = repository && repository.sourceRepository

  if (githubRepository) {
    data.repository = githubRepository
    data.primaryLanguage = githubRepository.primaryLanguage
    data.repositoryUrl = githubRepository.url
    data.stars = githubRepository.stargazers.totalCount
    data.forks = githubRepository.forks.totalCount
    data.watchers = githubRepository.watchers.totalCount
    data.pullRequests = githubRepository.pullRequests.totalCount
    data.issues = githubRepository.issues.totalCount
    data.homepage = data.homepage || githubRepository.homepageUrl
    data.defaultBranch = githubRepository.defaultBranchRef.name
    data.readmeUrl =
      data.repositoryUrl +
      '/tree/' +
      data.defaultBranch +
      '/' +
      githubRepository.readmeFilename
  }

  return data
}
