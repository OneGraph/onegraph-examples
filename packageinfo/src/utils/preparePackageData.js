export default function preparePackageData({
  repository,
  time,
  homepage,
  name,
  description,
  license,
  downloadsRange,
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
  const downloads = downloadsRange.downloads

  const dependencies = versions.find(
    version => version.version === currentVersion
  ).dependencies

  const data = {
    currentVersion,
    dependencies,
    lastPublishDate,
    versions: sortedVersions,
    homepage,
    name,
    description,
    primaryLanguage: {},
    downloads,
    maintainers,
    keywords: keywords || [],
    license,
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
