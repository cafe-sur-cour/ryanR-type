<template>
  <div class="min-h-screen bg-gray-900 p-4">
    <div class="max-w-6xl mx-auto space-y-6">
      <!-- Header -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <div class="flex justify-between items-center">
          <div>
            <h1 class="text-3xl font-bold text-white">R-Type Server Monitor</h1>
            <p class="text-gray-400 mt-1">Real-time server statistics and management</p>
          </div>
          <div class="flex space-x-3">
            <Button @click="refreshData" :disabled="loading" variant="default" class="flex items-center space-x-2 bg-blue-600 hover:bg-blue-700">
              <RefreshCwIcon class="h-4 w-4" :class="{ 'animate-spin': loading }" />
              <span>Refresh</span>
            </Button>
          </div>
        </div>
      </div>

      <!-- Status Overview -->
      <div class="grid grid-cols-1 md:grid-cols-5 gap-6">
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ServerIcon class="h-8 w-8 text-blue-400" />
            <div>
              <p class="text-sm text-gray-400">Server Status</p>
              <p :class="serverInfo.connectedClients !== undefined ? 'text-green-400' : 'text-red-400'" class="text-xl font-bold">
                {{ serverInfo.connectedClients !== undefined ? 'Online' : 'Offline' }}
              </p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <UsersIcon class="h-8 w-8 text-green-400" />
            <div>
              <p class="text-sm text-gray-400">Connected Clients</p>
              <p class="text-2xl font-bold text-white">{{ serverInfo.connectedClients || 0 }}</p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <GamepadIcon class="h-8 w-8 text-purple-400" />
            <div>
              <p class="text-sm text-gray-400">Active Lobbies</p>
              <p class="text-2xl font-bold text-white">{{ serverInfo.activeLobbies || 0 }}</p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ClockIcon class="h-8 w-8 text-yellow-400" />
            <div>
              <p class="text-sm text-gray-400">Uptime</p>
              <p class="text-2xl font-bold text-white">{{ formatUptime(serverInfo.uptime) }}</p>
            </div>
          </div>
        </div>

        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <div class="flex items-center space-x-3">
            <ActivityIcon class="h-8 w-8 text-purple-400" />
            <div>
              <p class="text-sm text-gray-400">TPS</p>
              <p class="text-2xl font-bold text-white">{{ serverInfo.tps || 0 }}</p>
            </div>
          </div>
        </div>
      </div>

      <!-- Detailed Information -->
      <div class="grid grid-cols-1 gap-6">
        <!-- Lobby Details -->
        <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
          <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
            <GamepadIcon class="h-5 w-5 text-purple-400" />
            <span>Lobby Details</span>
          </h2>
          <div v-if="serverInfo.lobbyDetails && serverInfo.lobbyDetails.length > 0" class="space-y-2">
            <div v-for="(lobby, index) in serverInfo.lobbyDetails" :key="index"
                 class="bg-gray-700 rounded p-3 cursor-pointer hover:bg-gray-600 transition-colors"
                 @click="toggleLobbyDetails(index)">
              <div class="flex justify-between items-center">
                <span class="text-gray-300">{{ lobby }}</span>
                <div class="flex items-center space-x-2">
                  <span class="text-sm text-gray-500">{{ getLobbyPlayerCount(index) }} players</span>
                  <svg class="w-4 h-4 text-gray-400 transform transition-transform"
                       :class="{ 'rotate-180': expandedLobbies.includes(index) }"
                       fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 9l-7 7-7-7"></path>
                  </svg>
                </div>
              </div>
              <!-- Expanded lobby player list -->
              <div v-if="expandedLobbies.includes(index)" class="mt-3 pt-3 border-t border-gray-600">
                <div v-if="serverInfo.lobbyPlayerDetails && serverInfo.lobbyPlayerDetails[index] && serverInfo.lobbyPlayerDetails[index].length > 0"
                     class="space-y-1">
                  <div v-for="(player, playerIndex) in serverInfo.lobbyPlayerDetails[index]" :key="playerIndex"
                       class="text-sm text-gray-400 bg-gray-800 rounded px-2 py-1">
                    {{ player }}
                  </div>
                </div>
                <div v-else class="text-sm text-gray-500">
                  No players in this lobby
                </div>
              </div>
            </div>
          </div>
          <div v-else class="text-gray-500 text-center py-4">
            No active lobbies
          </div>
        </div>

        <!-- Players Section -->
        <div class="grid grid-cols-1 md:grid-cols-3 gap-6">
          <!-- Logged in Players -->
          <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
            <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
              <UsersIcon class="h-5 w-5 text-green-400" />
              <span>Logged in Players</span>
            </h2>
            <div v-if="serverInfo.playerDetails && serverInfo.playerDetails.length > 0" class="space-y-2">
              <div v-for="(player, index) in serverInfo.playerDetails" :key="index">
                <div class="bg-gray-700 rounded p-3 cursor-pointer hover:bg-gray-600 transition-colors"
                     @click="togglePlayerDetails(index)">
                  <div class="flex justify-between items-center">
                    <span class="text-gray-300">{{ player }}</span>
                    <div class="flex items-center space-x-2">
                      <div class="w-2 h-2 bg-green-400 rounded-full"></div>
                      <svg class="w-4 h-4 text-gray-400 transform transition-transform"
                           :class="{ 'rotate-180': selectedPlayerIndex === index }"
                           fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 9l-7 7-7-7"></path>
                      </svg>
                    </div>
                  </div>
                </div>
                <!-- Player stats -->
                <div v-if="selectedPlayerIndex === index && serverInfo.playerStats && serverInfo.playerStats[index]"
                     class="mt-2 ml-4 p-3 bg-gray-800 rounded border-l-4 border-green-400">
                  <div class="grid grid-cols-4 gap-4 text-sm">
                    <div class="text-center">
                      <p class="text-gray-400">Games Played</p>
                      <p class="text-white font-bold text-lg">{{ serverInfo.playerStats[index].games_played || 0 }}</p>
                    </div>
                    <div class="text-center">
                      <p class="text-gray-400">Wins</p>
                      <p class="text-white font-bold text-lg">{{ serverInfo.playerStats[index].wins || 0 }}</p>
                    </div>
                    <div class="text-center">
                      <p class="text-gray-400">High Score</p>
                      <p class="text-white font-bold text-lg">{{ serverInfo.playerStats[index].high_score || 0 }}</p>
                    </div>
                    <div class="text-center">
                      <p class="text-gray-400">Banned</p>
                      <p :class="serverInfo.playerStats[index].banned ? 'text-red-400' : 'text-green-400'" class="font-bold text-lg">
                        {{ serverInfo.playerStats[index].banned ? 'Yes' : 'No' }}
                      </p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
            <div v-else class="text-gray-500 text-center py-4">
              No players logged in
            </div>
          </div>

          <!-- In Game Players -->
          <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
            <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
              <GamepadIcon class="h-5 w-5 text-red-400" />
              <span>In Game Players</span>
            </h2>
            <div v-if="getInGamePlayerIndices().length > 0" class="space-y-2">
              <div v-for="playerIndex in getInGamePlayerIndices()" :key="playerIndex">
                <div class="bg-gray-700 rounded p-3 cursor-pointer hover:bg-gray-600 transition-colors"
                     @click="togglePlayerDetails(playerIndex)">
                  <div class="flex justify-between items-center">
                    <span class="text-gray-300">{{ serverInfo.playerDetails?.[playerIndex] }}</span>
                    <div class="flex items-center space-x-2">
                      <div class="w-2 h-2 bg-red-400 rounded-full"></div>
                      <svg class="w-4 h-4 text-gray-400 transform transition-transform"
                           :class="{ 'rotate-180': selectedPlayerIndex === playerIndex }"
                           fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 9l-7 7-7-7"></path>
                      </svg>
                    </div>
                  </div>
                </div>
                <!-- Player stats -->
                <div v-if="selectedPlayerIndex === playerIndex && serverInfo.playerStats && serverInfo.playerStats[playerIndex]"
                     class="mt-2 ml-4 p-3 bg-gray-800 rounded border-l-4 border-red-400">
                  <div class="grid grid-cols-4 gap-4 text-sm">
                    <div class="text-center">
                      <p class="text-gray-400">Games Played</p>
                      <p class="text-white font-bold text-lg">{{ serverInfo.playerStats[playerIndex].games_played || 0 }}</p>
                    </div>
                    <div class="text-center">
                      <p class="text-gray-400">Wins</p>
                      <p class="text-white font-bold text-lg">{{ serverInfo.playerStats[playerIndex].wins || 0 }}</p>
                    </div>
                    <div class="text-center">
                      <p class="text-gray-400">High Score</p>
                      <p class="text-white font-bold text-lg">{{ serverInfo.playerStats[playerIndex].high_score || 0 }}</p>
                    </div>
                    <div class="text-center">
                      <p class="text-gray-400">Godmod</p>
                      <p :class="serverInfo.playerStats[playerIndex].godmod ? 'text-yellow-400' : 'text-gray-400'" class="font-bold text-lg">
                        {{ serverInfo.playerStats[playerIndex].godmod ? 'Yes' : 'No' }}
                      </p>
                    </div>
                  </div>
                </div>
              </div>
            </div>
            <div v-else class="text-gray-500 text-center py-4">
              No players in game
            </div>
          </div>

          <!-- Banned Players -->
          <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
            <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
              <BanIcon class="h-5 w-5 text-red-400" />
              <span>Banned Players</span>
            </h2>
            <div v-if="serverInfo.bannedPlayers && serverInfo.bannedPlayers.length > 0" class="space-y-2">
              <div v-for="(player, index) in serverInfo.bannedPlayers" :key="index"
                   class="bg-gray-700 rounded p-3">
                <div class="flex justify-between items-center">
                  <span class="text-gray-300">{{ player }}</span>
                  <div class="flex items-center space-x-2">
                    <div class="w-2 h-2 bg-red-400 rounded-full"></div>
                    <span class="text-sm text-red-400">Banned</span>
                  </div>
                </div>
              </div>
            </div>
            <div v-else class="text-gray-500 text-center py-4">
              No banned players
            </div>
          </div>
        </div>
      </div>

      <!-- System Information -->
      <div class="bg-gray-800 rounded-lg p-6 shadow-lg">
        <h2 class="text-xl font-bold text-white mb-4 flex items-center space-x-2">
          <InfoIcon class="h-5 w-5 text-blue-400" />
          <span>System Information</span>
        </h2>
        <div class="grid grid-cols-1 md:grid-cols-3 gap-4">
          <div class="bg-gray-700 rounded p-4">
            <p class="text-sm text-gray-400">Server IP</p>
            <p class="text-2xl font-bold text-white">{{ serverConfig.serverIp || 'localhost' }}</p>
          </div>
          <div class="bg-gray-700 rounded p-4">
            <p class="text-sm text-gray-400">HTTP Port</p>
            <p class="text-2xl font-bold text-white">{{ serverConfig.httpPort || 5173 }}</p>
          </div>
          <div class="bg-gray-700 rounded p-4">
            <p class="text-sm text-gray-400">Game Port</p>
            <p class="text-2xl font-bold text-white">{{ serverConfig.gamePort || 4242 }}</p>
          </div>
        </div>
      </div>

      <!-- Auto-refresh indicator -->
      <div class="fixed bottom-0 left-1/2 transform -translate-x-1/2 text-gray-500 text-sm bg-gray-900 px-4 py-2 rounded-t">
        Auto-refreshing every 5 seconds • Last updated: {{ lastUpdate }}
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import Button from './ui/Button.vue'
import { RefreshCwIcon, ServerIcon, UsersIcon, GamepadIcon, ClockIcon, InfoIcon, BanIcon, ActivityIcon } from 'lucide-vue-next'

interface Props {
  password: string
}

interface ServerInfo {
  connectedClients?: number
  uptime?: number
  activeLobbies?: number
  totalPlayers?: number
  lobbyDetails?: string[]
  playerDetails?: string[]
  lobbyPlayerDetails?: string[][]
  playerStats?: Array<Record<string, number>>
  inGamePlayers?: string[]
  bannedPlayers?: string[]
  tps?: number
}

interface ServerConfig {
  serverIp?: string
  httpPort?: number
  gamePort?: number
}

const props = defineProps<Props>()

const serverInfo = ref<ServerInfo>({})
const serverConfig = ref<ServerConfig>({})
const loading = ref(false)
const lastUpdate = ref('')
const autoRefreshInterval = ref<NodeJS.Timeout | null>(null)
const expandedLobbies = ref<number[]>([])
const selectedPlayerIndex = ref<number | null>(null)

const formatUptime = (seconds?: number): string => {
  if (!seconds) return '0s'
  const hours = Math.floor(seconds / 3600)
  const minutes = Math.floor((seconds % 3600) / 60)
  const secs = seconds % 60

  if (hours > 0) return `${hours}h ${minutes}m ${secs}s`
  if (minutes > 0) return `${minutes}m ${secs}s`
  return `${secs}s`
}

const updateLastUpdate = () => {
  lastUpdate.value = new Date().toLocaleTimeString()
}

const fetchServerConfig = async () => {
  try {
    const response = await fetch('/api/config', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })

    if (response.ok) {
      serverConfig.value = await response.json()
    } else if (response.status === 401) {
      alert('Session expired. Please login again.')
      window.location.reload()
    }
  } catch (error) {
    console.error('Error fetching server config:', error)
  }
}

const fetchServerInfo = async () => {
  loading.value = true
  try {
    const response = await fetch('/api/info', {
      headers: {
        'Authorization': `Bearer ${props.password}`
      }
    })

    if (response.ok) {
      serverInfo.value = await response.json()
      updateLastUpdate()
    } else if (response.status === 401) {
      alert('Session expired. Please login again.')
      window.location.reload()
    } else {
      console.error('Failed to fetch server info')
    }
  } catch (error) {
    console.error('Error fetching server info:', error)
  } finally {
    loading.value = false
  }
}

const refreshData = () => {
  fetchServerInfo()
}

const toggleLobbyDetails = (index: number) => {
  const currentIndex = expandedLobbies.value.indexOf(index)
  if (currentIndex > -1) {
    expandedLobbies.value.splice(currentIndex, 1)
  } else {
    expandedLobbies.value.push(index)
  }
}

const getLobbyPlayerCount = (index: number): number => {
  if (serverInfo.value.lobbyPlayerDetails && serverInfo.value.lobbyPlayerDetails[index]) {
    return serverInfo.value.lobbyPlayerDetails[index].length
  }
  return 0
}

const togglePlayerDetails = (index: number) => {
  selectedPlayerIndex.value = selectedPlayerIndex.value === index ? null : index
}

const getInGamePlayerIndices = (): number[] => {
  const inGameIndices: number[] = []

  if (serverInfo.value.inGamePlayers && serverInfo.value.playerDetails) {
    const playerIndexMap = new Map<string, number>()
    serverInfo.value.playerDetails.forEach((player, index) => {
      playerIndexMap.set(player, index)
    })

    serverInfo.value.inGamePlayers.forEach(playerDetail => {
      const playerIndex = playerIndexMap.get(playerDetail)
      if (playerIndex !== undefined) {
        inGameIndices.push(playerIndex)
      }
    })
  }

  return inGameIndices
}

const startAutoRefresh = () => {
  autoRefreshInterval.value = setInterval(() => {
    fetchServerInfo()
  }, 5000)
}

const stopAutoRefresh = () => {
  if (autoRefreshInterval.value) {
    clearInterval(autoRefreshInterval.value)
    autoRefreshInterval.value = null
  }
}

onMounted(() => {
  fetchServerConfig()
  fetchServerInfo()
  startAutoRefresh()
})

onUnmounted(() => {
  stopAutoRefresh()
})
</script>

<style scoped>
</style>